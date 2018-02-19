#include <iostream>
#include "Mesh.hpp"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>

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
   std::cout<<node.name << " has " << node.numChildren << " children " << "and index: " << node.index << std::endl;
   if (node.numChildren==0) return;
   for ( auto child : node.children) {
      printSkeleton(child);
   }
}

void
addWeight(float weight, int id, glm::ivec3 & boneIds, glm::fvec3 & weights) {
   for (int i=0; i<3; ++i) {
      if (weights[i] == 0.0f) {
         weights[i] = weight;
         boneIds[i] = id;
      }
   }
   std::cout << boneIds.x << " " << weights.x << " " << boneIds.y << " " << weights.y << std::endl;
}

void
Mesh::animateSkeleton(SkeletonNode & node,
                      glm::mat4 & parentMat,
                      std::vector<glm::mat4> & offsetMats,
                      std::vector<glm::mat4> & boneAnimationMats,
                      float animationTime) {

   glm::mat4 localMat(1.f);
   glm::mat4 ourMat = parentMat;
   //Iterpolate transformation matrix
   animationTime = std::fmod(animationTime,0.833f);

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
   //TODO Scale

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

      auto pR     = node.rotations[prev];
      auto nR     = node.rotations[next];
      glm::quat q = glm::mix(pR,nR,t);
      rotNode     = glm::toMat4(q);
   }

   localMat = transNode*rotNode;

   int boneIdx = node.index;
   if (boneIdx>-1) {
      ourMat = parentMat *  localMat * offsetMats[boneIdx];
      boneAnimationMats[boneIdx]=ourMat;
      //Debug
//      auto r = glm::column(boneAnimationMats[boneIdx],3);
//      m_BonePos[boneIdx] = glm::vec3(r.x, r.y, r.z);
   }

   for (auto & child : node.children) {
      animateSkeleton(child, ourMat, offsetMats, boneAnimationMats, animationTime);
   }


}

void
Mesh::loadMesh(std::string path, std::vector<int> & boneIds, double & animationDuration) {

   Assimp::Importer importer;
   auto scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenNormals);

   if (!scene) {
      std::cout << "Could not load scene! \n";
      return;
   }

   if (!scene->HasMeshes()) {
      std::cout << "Scene is without meshes! \n";
      return;
   }

   auto mesh = scene->mMeshes[0]; // One mesh is enough at the moment

   if (mesh->HasPositions() & mesh->HasNormals()) {
      for (std::size_t i=0; i<mesh->mNumVertices; ++i) {
         VertexN vec;
         copyVector(mesh->mVertices[i], vec.Position);
         copyVector(mesh->mNormals[i],vec.Normal);
         m_Mesh.push_back(vec);
      }
   }

   if (mesh->HasBones()) {
      boneIds.resize(m_Mesh.size());
      for (int r=0; r<m_Mesh.size(); ++r) {
         m_BoneIds.emplace_back(0);
         m_Weights.emplace_back(0.f);
      }
      for (std::size_t i=0; i<mesh->mNumBones; ++i)  {
         auto bone = mesh->mBones[i];
         m_BoneOffSet.emplace_back(1.f);
         copyMatrix(bone->mOffsetMatrix,m_BoneOffSet.back());
         boneName.emplace_back(IndexedBoneName{bone->mName.C_Str(), i});

         for (std::size_t j=0; j<bone->mNumWeights; ++j) {
            auto weight = bone->mWeights[j];
            addWeight(weight.mWeight,i,m_BoneIds[weight.mVertexId], m_Weights[weight.mVertexId]);

         }
         //For Debug Visualization
         auto r = glm::column(m_BoneOffSet.back(),3);
         m_BonePos.emplace_back(r.x, r.y, r.z);
      }
      boneIds.shrink_to_fit();
   }

   //Create own skeleton data structure to get rid of assimp during runtime
   auto root = scene->mRootNode;
   importSkeletonNode(root,m_Skeleton,-1,boneName);
   printSkeleton(m_Skeleton);

   if (scene->mNumAnimations <=0) {
      std::cout << "There are no animatoins\n";
      return;
   }

   auto anim = scene->mAnimations[0]; // First animation;
   std::cout << "Animation name: " << anim->mName.C_Str() << std::endl;
   std::cout << "Animation channel: " << anim->mNumChannels << std::endl;
   std::cout << "Animation duration: " << anim->mDuration << std::endl;
   std::cout << "Animation mesh channels: " << anim->mNumMeshChannels << std::endl;
   std::cout << "Animation ticks per second: " << anim->mTicksPerSecond << std::endl;

   animationDuration = anim->mDuration;

   for (std::size_t i=0; i<anim->mNumChannels; ++i) {
      auto channel = anim->mChannels[i];
      SkeletonNode * n = findNode(m_Skeleton, channel->mNodeName.C_Str());
      if (!n) continue;

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

void
Mesh::importSkeletonNode(aiNode * assimpNode,
                   SkeletonNode & skNode,
                   int            index,
                   std::vector<IndexedBoneName> const & names) {

   if (!assimpNode) {
      std::cout << "Node is nullptr \n";
      return;
   }
//   std::cout << "Name:" << assimpNode->mName.C_Str() << std::endl;
//   std::cout << "Nr of Childs:" << assimpNode->mNumChildren << std::endl;
//   std::cout << "Index in con:" << index << std::endl;
   bool skip = false;
   for (auto const & bN : names) {
      if (bN.name.compare(assimpNode->mName.C_Str())!=0) continue;

      skNode.name        = assimpNode->mName.C_Str();
      skNode.numChildren = assimpNode->mNumChildren;
      skNode.index       = bN.index;
      if (assimpNode->mNumChildren == 0) return;

      for (std::size_t i=0; i<assimpNode->mNumChildren; ++i) {
         skNode.children.emplace_back();
         importSkeletonNode(assimpNode->mChildren[i], skNode.children.back(), i, names);
      }
      skip = true;

   }
   if (skip) return;

   for (std::size_t i=0; i<assimpNode->mNumChildren; ++i) {
      importSkeletonNode(assimpNode->mChildren[i], skNode, index, names);
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
