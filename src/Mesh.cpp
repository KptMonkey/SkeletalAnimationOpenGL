#include <iostream>
#include "Mesh.hpp"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

void
copyMatrix(aiMatrix4x4 const & aMatrix, glm::mat4 & mat) {
   assert(sizeof(mat) == sizeof(aMatrix));
   memcpy(&mat, &aMatrix, sizeof(aMatrix));
   mat = glm::transpose(mat);
}

void
copyVector(aiVector3D const & aVector, glm::vec3 & vec) {
   assert(sizeof(vec)==sizeof(aVector));
   memcpy(&vec, &aVector, sizeof(aVector));
}

void
printSkeleton(SkeletonNode const & node) {
   if (node.numChildren==0) return;
   for ( auto child : node.children) {
      printSkeleton(child);
   }
}

void
addWeight(float weight, int id, glm::ivec4 & boneIds, glm::vec4 & weights) {
   for (int i=0; i<4; ++i) {
      if (weights[i] == 0.0 ) {
         weights[i] = weight;
         boneIds[i] = id;
         break;
      }
   }
}

void
Mesh::animate(SkeletonNode & node,
              glm::mat4  const & parentMat,
              std::vector<glm::mat4> & boneAnimationMats,
              float animationTime,
              std::string animationName) {

   glm::mat4 localMat(1.f);
   glm::mat4 ourMat = parentMat;
   float animationDuration = m_AnimationInfo[animationName].animationDuration;
   animationTime = std::fmod(animationTime, animationDuration);

   // Make 1 function out of the transformation calculations...
   auto const & trfm = node.animations[animationName];
   glm::mat4 transNode(1.f);
   if (trfm.numTrans > 0) {
      int prev = 0;
      int next = 0;
      for (int i=0; i<trfm.numTrans-1; ++i) {
         prev = i;
         next = i+1;
         if (trfm.transTimes[next]>=animationTime) break;
      }

      float total = trfm.transTimes[next]-trfm.transTimes[prev];
      float t     = (animationTime-trfm.transTimes[prev])/total;

      auto pT     = trfm.translations[prev];
      auto nT     = trfm.translations[next];
      auto lerped = pT*(1.f-t) + nT*t;

      transNode = glm::translate(transNode, lerped);
   }

   glm::mat4 scaleNode(1.f);
   if (trfm.numScale > 0) {
      int prev = 0;
      int next = 0;
      for (int i=0; i<trfm.numScale-1; ++i) {
         prev = i;
         next = i+1;
         if (trfm.scaleTimes[next]>=animationTime) break;
      }

      float total = trfm.scaleTimes[next]-trfm.scaleTimes[prev];
      float t     = (animationTime-trfm.scaleTimes[prev])/total;

      auto pT     = trfm.scales[prev];
      auto nT     = trfm.scales[next];
      auto lerped = pT*(1.f-t) + nT*t;

      scaleNode = glm::scale(scaleNode, lerped);
   }

   glm::mat4 rotNode(1.f);
   if (trfm.numRot > 0) {
      int prev = 0;
      int next = 0;
      for (int i=0; i<trfm.numTrans-1; ++i) {
         prev = i;
         next = i+1;
         if (trfm.rotTimes[next]>=animationTime) break;
      }

      float total_t = trfm.rotTimes[next]-trfm.rotTimes[prev];
      float t       = (animationTime-trfm.rotTimes[prev])/total_t;

      auto pR = trfm.rotations[prev];
      auto nR = trfm.rotations[next];
      // Rotation has to be done in positive direction!
      if (glm::dot(pR,nR)<0.f) {
         pR *=-1.f;
      }
      glm::quat q = glm::mix(pR,nR,t);
      rotNode     = glm::toMat4(q);
   }

   auto const & animatedBones = m_AnimationInfo[animationName].animatedBones;
   auto n = std::find(std::begin(animatedBones), std::end(animatedBones),node.name);

   if (n!=std::end(animatedBones)) localMat = transNode * rotNode * scaleNode;
   else                                              localMat = node.relTransformation;

   ourMat = parentMat * localMat;
   boneAnimationMats[node.index]=ourMat*m_BoneOffSet[node.index];
   for (auto & child : node.children) {
      animate(child, ourMat, boneAnimationMats, animationTime,animationName);
   }

}

void
Mesh::loadMesh(std::string path) {

   Assimp::Importer importer;
   auto scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

   if (!scene) {
      std::cout << "Could not load scene! \n";
      return;
   }

   if (!scene->HasMeshes()) {
      std::cout << "Scene is without meshes! \n";
      return;
   }
   int vert = 0;
   int bid = 0;

   for (std::size_t k =0; k<scene->mNumMeshes; ++k) {
      auto const & mesh = scene->mMeshes[k];

      if (!mesh->HasPositions() && !mesh->HasNormals()) {
         std::cout << "Has no Vertices or Normals\n";
         return;
      }
      for (std::size_t i=0; i<mesh->mNumVertices; ++i) {
         VertexS sec;

         copyVector(mesh->mVertices[i], sec.Position);
         copyVector(mesh->mNormals[i],sec.Normal);
         m_Mesh.push_back(sec);
      }

      for ( std::size_t i=0; i<mesh->mNumFaces; ++i) {
         aiFace& face = mesh->mFaces[i];
         m_Index.push_back(vert+face.mIndices[0]);
         m_Index.push_back(vert+face.mIndices[1]);
         m_Index.push_back(vert+face.mIndices[2]);
      }
      if (!mesh->HasBones()) {
         std::cout << "Has no Bones\n";
         return;
      }
      for (std::size_t i=0; i<mesh->mNumBones; ++i)  {
         auto const & bone = mesh->mBones[i];
         m_BoneOffSet.emplace_back(1.f);
         copyMatrix(bone->mOffsetMatrix,m_BoneOffSet.back());
         auto boneIndex = m_BoneIndex.find(bone->mName.C_Str());

         if (boneIndex == std::end(m_BoneIndex)) {
            m_BoneIndex[bone->mName.C_Str()] = i + bid;
            for (std::size_t j=0; j<bone->mNumWeights; ++j) {
               auto weight = bone->mWeights[j];
               addWeight(weight.mWeight, i+bid,
                         m_Mesh[weight.mVertexId+vert].BoneId, m_Mesh[weight.mVertexId+vert].Weight);
            }
         }
         else {
            for (std::size_t j=0; j<bone->mNumWeights; ++j) {
               auto weight = bone->mWeights[j];
               addWeight(weight.mWeight, boneIndex->second,
                         m_Mesh[weight.mVertexId+vert].BoneId, m_Mesh[weight.mVertexId+vert].Weight);
            }
         }
         //For Debug Visualization
         auto r = glm::column(m_BoneOffSet.back(),3);
         m_BonePos.emplace_back(r.x, r.y, r.z);
      }
      bid  += mesh->mNumBones;
      vert += mesh->mNumVertices;
   }

   //TODO: This stuff shouldn't be in the loadMesh function
   auto root = scene->mRootNode;
   createSkeleton(root, m_Skeleton);
//   printSkeleton(m_Skeleton); //Debug
   createAnimation(scene);
}

void
Mesh::createSkeleton(aiNode * assimpNode, SkeletonNode & skNode) {

   if (!assimpNode) {
      std::cout << "Node is nullptr \n";
      return;
   }
   bool skip = false;

   auto boneIndex = m_BoneIndex.find(assimpNode->mName.C_Str());

   if (boneIndex!=std::end(m_BoneIndex)) {
      skNode.name        = assimpNode->mName.C_Str();
      skNode.numChildren = assimpNode->mNumChildren;
      skNode.index       = boneIndex->second;
      copyMatrix(assimpNode->mTransformation,skNode.relTransformation);

      if (assimpNode->mNumChildren == 0) return;

      for (std::size_t i=0; i<assimpNode->mNumChildren; ++i) {
         if (m_BoneIndex.find(assimpNode->mName.C_Str()) == std::end(m_BoneIndex)) continue;
         skNode.children.emplace_back();
         createSkeleton(assimpNode->mChildren[i], skNode.children.back());
      }
      skip = true;
   }

   if (skip) return;

   for (std::size_t i=0; i<assimpNode->mNumChildren; ++i) {
      createSkeleton(assimpNode->mChildren[i], skNode);
   }
}

void
Mesh::createAnimation(aiScene const * scene) {
   if (scene->mNumAnimations <=0) {
      std::cout << "There are no animations\n";
      return;
   }
   std::cout << scene->mNumAnimations << std::endl;
   for (int i =0; i < scene->mNumAnimations; ++i) {
      auto anim = scene->mAnimations[i];
      std::cout << anim->mName.C_Str() << std::endl;
   }

   for (int a =0; a < scene->mNumAnimations; ++a) {
      auto anim = scene->mAnimations[a];
      auto const & animName = anim->mName.C_Str();
      m_AnimationInfo[animName].animationDuration = anim->mDuration;

      for (std::size_t i=0; i<anim->mNumChannels; ++i) {
         auto channel = anim->mChannels[i];
         SkeletonNode * n = findNode(m_Skeleton, channel->mNodeName.C_Str());
         if (!n) continue;

         m_AnimationInfo[animName].animatedBones.emplace_back(n->name);

         auto & t = n->animations[animName];

         for (std::size_t p = 0; p<channel->mNumPositionKeys; ++p) {
            glm::vec3 vec;
            copyVector(channel->mPositionKeys[p].mValue,vec);
            t.translations.emplace_back(vec);
            t.transTimes.emplace_back(channel->mPositionKeys[p].mTime);
            t.numTrans = channel->mNumPositionKeys;;
         }

         for (std::size_t s = 0; s<channel->mNumScalingKeys; ++s) {
            glm::vec3 vec;
            copyVector(channel->mScalingKeys[s].mValue,vec);
            t.scales.emplace_back(vec);
            t.scaleTimes.emplace_back(channel->mScalingKeys[s].mTime);
            t.numScale = channel->mNumScalingKeys;
         }

         for (std::size_t r = 0; r<channel->mNumRotationKeys; ++r) {
            glm::quat q;
            auto rot = channel->mRotationKeys[r].mValue;
            q.x = rot.x;
            q.y = rot.y;
            q.z = rot.z;
            q.w = rot.w;
            t.rotations.emplace_back(q);
            t.rotTimes.emplace_back(channel->mRotationKeys[r].mTime);
            t.numRot = channel->mNumRotationKeys;
         }
      }
   }
}

SkeletonNode *
Mesh::findNode(SkeletonNode & node, std::string name) {
   if(name==node.name) {
      return &node;
   }
   for (auto & child : node.children) {
      auto n = findNode(child,name);
      if (n) return n;
   }
   return  nullptr;
}
