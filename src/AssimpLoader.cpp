#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "AssimpLoader.hpp"

SkeletonNode *
findNode(SkeletonNode & node, std::string name) {
   if(name==node.name) {
      return &node;
   }
   for (auto & child : node.children) {
      auto n = findNode(child,name);
      if (n) return n;
   }
   return  nullptr;
}

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
AssimpLoader::loadMesh(Mesh & mesh, std::string path) {
   m_Scene = m_Importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

   if (!m_Scene) {
      std::cout << "Could not load scene! \n";
      return;
   }

   if (!m_Scene->HasMeshes()) {
      std::cout << "Scene is without meshes! \n";
      return;
   }
   int vert = 0;
   int bid  = 0;

   for (std::size_t k=0; k<m_Scene->mNumMeshes; ++k) {
      auto const & aMesh = m_Scene->mMeshes[k];

      if (!aMesh->HasPositions() && !aMesh->HasNormals()) {
         std::cout << "Has no Vertices or Normals\n";
         return;
      }
      for (std::size_t i=0; i<aMesh->mNumVertices; ++i) {
         AnimationVertex sec;

         copyVector(aMesh->mVertices[i], sec.position);
         copyVector(aMesh->mNormals[i], sec.normal);
         mesh.mesh.push_back(sec);
      }

      for ( std::size_t i=0; i<aMesh->mNumFaces; ++i) {
         aiFace& face = aMesh->mFaces[i];
         mesh.indexVec.push_back(vert+face.mIndices[0]);
         mesh.indexVec.push_back(vert+face.mIndices[1]);
         mesh.indexVec.push_back(vert+face.mIndices[2]);
      }
      if (!aMesh->HasBones()) {
         std::cout << "Has no Bones\n";
         return;
      }
      for (std::size_t i=0; i<aMesh->mNumBones; ++i)  {
         auto const & bone = aMesh->mBones[i];
         mesh.boneOffSet.emplace_back(1.f);
         copyMatrix(bone->mOffsetMatrix,mesh.boneOffSet.back());
         auto boneIndex = mesh.boneIndex.find(bone->mName.C_Str());

         if (boneIndex == std::end(mesh.boneIndex)) {
            mesh.boneIndex[bone->mName.C_Str()] = i + bid;
            for (std::size_t j=0; j<bone->mNumWeights; ++j) {
               auto weight = bone->mWeights[j];
               addWeight(weight.mWeight, i+bid,
                         mesh.mesh[weight.mVertexId+vert].boneId, mesh.mesh[weight.mVertexId+vert].weight);
            }
         }
         else {
            for (std::size_t j=0; j<bone->mNumWeights; ++j) {
               auto weight = bone->mWeights[j];
               addWeight(weight.mWeight, boneIndex->second,
                         mesh.mesh[weight.mVertexId+vert].boneId, mesh.mesh[weight.mVertexId+vert].weight);
            }
         }
         //For Debug Visualization
         auto r = glm::column(mesh.boneOffSet.back(), 3);
         mesh.bonePos.emplace_back(r.x, r.y, r.z);
      }
      bid  += aMesh->mNumBones;
      vert += aMesh->mNumVertices;
   }
}

void
createSkeleton(aiNode * assimpNode, SkeletonNode & skNode, Mesh const & mesh) {
   if (!assimpNode) {
      std::cout << "Node is nullptr \n";
      return;
   }
   bool skip = false;

   auto boneIndex = mesh.boneIndex.find(assimpNode->mName.C_Str());

   if (boneIndex!=std::end(mesh.boneIndex)) {
      skNode.name        = assimpNode->mName.C_Str();
      skNode.numChildren = assimpNode->mNumChildren;
      skNode.index       = boneIndex->second;
      copyMatrix(assimpNode->mTransformation,skNode.relTransformation);

      if (assimpNode->mNumChildren == 0) return;

      for (std::size_t i=0; i<assimpNode->mNumChildren; ++i) {
         if (mesh.boneIndex.find(assimpNode->mName.C_Str()) == std::end(mesh.boneIndex)) continue;
         skNode.children.emplace_back();
         createSkeleton(assimpNode->mChildren[i], skNode.children.back(), mesh);
      }
      skip = true;
   }

   if (skip) return;

   for (std::size_t i=0; i<assimpNode->mNumChildren; ++i) {
      createSkeleton(assimpNode->mChildren[i], skNode, mesh);
   }
}

void
AssimpLoader::loadAnimation(Animation &animation, Mesh const & mesh) {

   auto root       = m_Scene->mRootNode;
   createSkeleton(root, animation.skeleton, mesh);

   if (m_Scene->mNumAnimations <=0) {
      std::cout << "There are no animations\n";
      return;
   }
   std::cout << m_Scene->mNumAnimations << std::endl;
   for (std::size_t i=0; i < m_Scene->mNumAnimations; ++i) {
      auto anim = m_Scene->mAnimations[i];
      std::cout << anim->mName.C_Str() << std::endl;
   }

   for (std::size_t a=0; a < m_Scene->mNumAnimations; ++a) {
      auto anim = m_Scene->mAnimations[a];
      auto const & animName = anim->mName.C_Str();
      animation.animationData[animName].animationDuration = anim->mDuration;
      animation.animationData[animName].name = animName;

      for (std::size_t i=0; i<anim->mNumChannels; ++i) {
         auto channel = anim->mChannels[i];
         SkeletonNode * n = findNode(animation.skeleton, channel->mNodeName.C_Str());
         if (!n) continue;

         animation.animationData[animName].animatedBones.emplace_back(n->name);

         auto & t = n->animations[animName];

         for (std::size_t p=0; p<channel->mNumPositionKeys; ++p) {
            glm::vec3 vec;
            copyVector(channel->mPositionKeys[p].mValue,vec);
            t.translations.emplace_back(vec);
            t.transTimes.emplace_back(channel->mPositionKeys[p].mTime);
            t.numTrans = channel->mNumPositionKeys;;
         }

         for (std::size_t s=0; s<channel->mNumScalingKeys; ++s) {
            glm::vec3 vec;
            copyVector(channel->mScalingKeys[s].mValue,vec);
            t.scales.emplace_back(vec);
            t.scaleTimes.emplace_back(channel->mScalingKeys[s].mTime);
            t.numScale = channel->mNumScalingKeys;
         }

         for (std::size_t r=0; r<channel->mNumRotationKeys; ++r) {
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

