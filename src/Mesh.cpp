#include <iostream>
#include "Mesh.hpp"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

void
copyMatrix(aiMatrix4x4 aMatrix, glm::mat4 & mat) { //The copy fixes a crash...
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
Mesh::animateSkeleton(SkeletonNode & node,
                      glm::mat4  const & parentMat,
                      std::vector<glm::mat4> & boneAnimationMats,
                      float animationTime) {

   glm::mat4 localMat(1.f);
   glm::mat4 ourMat = parentMat;
   animationTime = std::fmod(animationTime,m_AnimationDuration);

   // Make 1 function out of the transformation calculations...
   glm::mat4 transNode(1.f);
   if (node.numTrans > 0) {
      int prev = 0;
      int next = 0;
      for (int i=0; i<node.numTrans-1; ++i) {
         prev = i;
         next = i+1;
         if (node.transTimes[next]>=animationTime) break;
      }

      float total = node.transTimes[next]-node.transTimes[prev];
      float t     = (animationTime-node.transTimes[prev])/total;

      auto pT     = node.transformations[prev];
      auto nT     = node.transformations[next];
      auto lerped = pT*(1.f-t) + nT*t;

      transNode = glm::translate(transNode, lerped);
   }

   glm::mat4 scaleNode(1.f);
   if (node.numScale > 0) {
      int prev = 0;
      int next = 0;
      for (int i=0; i<node.numScale-1; ++i) {
         prev = i;
         next = i+1;
         if (node.scaleTimes[next]>=animationTime) break;
      }

      float total = node.scaleTimes[next]-node.scaleTimes[prev];
      float t     = (animationTime-node.scaleTimes[prev])/total;

      auto pT     = node.scales[prev];
      auto nT     = node.scales[next];
      auto lerped = pT*(1.f-t) + nT*t;

      scaleNode = glm::scale(scaleNode, lerped);
   }

   glm::mat4 rotNode(1.f);
   if (node.numRot > 0) {
      int prev = 0;
      int next = 0;
      for (int i=0; i<node.numTrans-1; ++i) {
         prev = i;
         next = i+1;
         if (node.rotTimes[next]>=animationTime) break;
      }

      float total_t = node.rotTimes[next]-node.rotTimes[prev];
      float t       = (animationTime-node.rotTimes[prev])/total_t;

      auto pR = node.rotations[prev];
      auto nR = node.rotations[next];
      // Rotation has to be done in positive direction!
      if (glm::dot(pR,nR)<0.f) {
         pR *=-1.f;
      }
      glm::quat q = glm::mix(pR,nR,t);

      rotNode     = glm::toMat4(q);
   }

   auto n = std::find(std::begin(m_AnimatedBones), std::end(m_AnimatedBones),node.name);

   if (n!=std::end(m_AnimatedBones)) localMat = transNode * rotNode * scaleNode;
   else                              localMat = node.relTransformation;

   ourMat = parentMat * localMat;
   boneAnimationMats[node.index]=ourMat*m_BoneOffSet[node.index];
   for (auto & child : node.children) {
      animateSkeleton(child, ourMat, boneAnimationMats, animationTime);
   }
}

void
Mesh::loadMesh(std::string path) {

   Assimp::Importer importer;
   auto scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices  );

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
      std::cout << "There are no animatoins\n";
      return;
   }
   std::cout << scene->mNumAnimations << std::endl;
   auto anim = scene->mAnimations[0]; // First animation;
   m_AnimationDuration = anim->mDuration;
   std::cout <<anim->mNumChannels << std::endl;

   for (std::size_t i=0; i<anim->mNumChannels; ++i) {
      auto channel = anim->mChannels[i];
      SkeletonNode * n = findNode(m_Skeleton, channel->mNodeName.C_Str());
      if (!n) continue;
      m_AnimatedBones.emplace_back(n->name);

      for (std::size_t p = 0; p<channel->mNumPositionKeys; ++p) {
         glm::vec3 vec;
         copyVector(channel->mPositionKeys[p].mValue,vec);
         n->transformations.emplace_back(vec);
         n->transTimes.emplace_back(channel->mPositionKeys[p].mTime);
         n->numTrans = channel->mNumPositionKeys;;
      }

      for (std::size_t s = 0; s<channel->mNumScalingKeys; ++s) {
         glm::vec3 vec;
         copyVector(channel->mScalingKeys[s].mValue,vec);
         n->scales.emplace_back(vec);
         n->scaleTimes.emplace_back(channel->mScalingKeys[s].mTime);
         n->numScale = channel->mNumScalingKeys;
      }

      for (std::size_t r = 0; r<channel->mNumRotationKeys; ++r) {
         glm::quat q;
         auto rot = channel->mRotationKeys[r].mValue;
         q.x = rot.x;
         q.y = rot.y;
         q.z = rot.z;
         q.w = rot.w;
         n->rotations.emplace_back(q);
         n->rotTimes.emplace_back(channel->mRotationKeys[r].mTime);
         n->numRot = channel->mNumRotationKeys;
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
