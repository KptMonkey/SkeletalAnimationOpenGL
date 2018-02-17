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
   std::cout<<node.name << " has " << node.numChildren << " children " << "and index: " << node.boneIndex << std::endl;
   if (node.numChildren==0) return;
   for ( auto child : node.children) {
      printSkeleton(child);
   }
}

void
Mesh::animateSkeleton(SkeletonNode & node,
                glm::mat4 & parentMat,
                std::vector<glm::mat4> & offsetMats,
                std::vector<glm::mat4> & boneAnimationMats,
                std::vector<glm::mat4> & globalAnim,
                float animationTime) {

   glm::mat4 localMat(1.f);
   glm::mat4 ourMat = parentMat;
   //Iterpolate transformation matrix
   animationTime = std::fmod(animationTime,0.833f);
   std::cout << "animationtime: "<< animationTime << std::endl;
   glm::mat4 nodeT(1.f);
   if (node.numPos > 0) {
      int prevKey = 0;
      int nextKey = 0;
      for (int i=0; i<node.numPos-1; ++i) {
         prevKey = i;
         nextKey = i+1;
         if (node.posTimes[nextKey]>=animationTime) break;
      }

      float total_t = node.posTimes[nextKey]-node.posTimes[prevKey];
      float t = (animationTime-node.posTimes[prevKey])/total_t;
      std::cout << "posT: " << node.posTimes[nextKey] << std::endl;
      std::cout << "posP: " << node.posTimes[prevKey] << std::endl;
      std::cout << "total_t: " << total_t << std::endl;
      std::cout << "t: " << t << std::endl;

      auto np = node.positions[prevKey];
      auto nn = node.positions[nextKey];
      auto lerped = np * (1.f-t) + nn*t;

      nodeT = glm::translate(nodeT, lerped);
   }
   //TODO Scale

   glm::mat4 nodeR(1.f);

   if (node.numRot > 0) {
      int prevKey = 0;
      int nextKey = 0;
      for (int i=0; i<node.numPos-1; ++i) {
         prevKey = i;
         nextKey = i+1;
         if (node.rotTimes[nextKey]>=animationTime) break;
      }

      float total_t = node.rotTimes[nextKey]-node.rotTimes[prevKey];
      float t = (animationTime-node.rotTimes[prevKey])/total_t;

      auto np = node.rotations[prevKey];
      auto nn = node.rotations[nextKey];
      glm::quat q = glm::mix(np,nn,t);
      nodeR = glm::toMat4(q);
   }

   localMat = nodeT*nodeR;

   int boneIdx = node.boneIndex;
   if (boneIdx>-1) {
      glm::mat4 boneOffset = offsetMats[boneIdx];
//      glm::mat4 invOffSet  = glm::inverse(boneOffset);
//      ourMat = parentMat * invOffSet * localMat * boneOffset;
      ourMat = parentMat *  localMat * boneOffset;
      boneAnimationMats[boneIdx]=ourMat;
   }

   for (auto & child : node.children) {
      animateSkeleton(child,ourMat,offsetMats, boneAnimationMats,globalAnim,animationTime);
   }


}

void
Mesh::loadMesh(std::string path, std::vector<int> & boneIds, double & animationDuration) {
   Assimp::Importer importer;
   auto scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate);

   if (!scene) {
      std::cout << "Could not load scene! \n";
      return;
   }

   if (!scene->HasMeshes()) std::cout << "Scene is without meshes! \n";

   auto mesh = scene->mMeshes[0]; // One mesh is enough at the moment

   if (!mesh->HasBones())     std::cout << "Mesh has no bones \n";
   if (!mesh->HasPositions()) std::cout << "Mesh has no positions \n";
   if (!mesh->HasNormals())   std::cout << "Mesh has no normals \n";

   for (std::size_t i=0; i<mesh->mNumVertices; ++i) {
      VertexN vec;
      copyVector(mesh->mVertices[i], vec.Position);
      copyVector(mesh->mNormals[i],vec.Normal);
      m_Mesh.push_back(vec);
   }
   boneIds.resize(m_Mesh.size());

   for (std::size_t i=0; i<mesh->mNumBones; ++i)  {
      auto bone = mesh->mBones[i];
      m_BoneOffSet.emplace_back(1.f);
      copyMatrix(bone->mOffsetMatrix,m_BoneOffSet.back());
      boneName.emplace_back(bone->mName.C_Str());
      auto r = glm::column(m_BoneOffSet.back(),3);
      m_BonePos.emplace_back(r.x, r.y, r.z);
      for (std::size_t j=0; j<bone->mNumWeights; ++j) {
         auto weight = bone->mWeights[j];
         if (weight.mWeight>0.5f) boneIds[weight.mVertexId]=i;
      }
   }
   boneIds.shrink_to_fit();

   //Create own skeleton data structure to get rid of assimp during runtime
   auto root = scene->mRootNode;
   importSkeletonNode(root,m_Skeleton,2,boneName);
   printSkeleton(m_Skeleton);

   if (scene->mNumAnimations <=0) std::cout << "There are no animatoins\n";

   auto anim = scene->mAnimations[0]; // First animation;
   std::cout << "Animation name: " << anim->mName.C_Str() << std::endl;
   std::cout << "Animation channel: " << anim->mNumChannels << std::endl;
   std::cout << "Animation duration: " << anim->mDuration << std::endl;
   std::cout << "Animation mesh channels: " << anim->mNumMeshChannels << std::endl;
   std::cout << "Animation ticks per second: " << anim->mTicksPerSecond << std::endl;

   animationDuration = anim->mDuration;

   for (int i=0; i<anim->mNumChannels; ++i) {
      auto channel = anim->mChannels[i];
      SkeletonNode * n = findNode(m_Skeleton, channel->mNodeName.C_Str());
      if (!n) continue;
      for (int p = 0; p<channel->mNumPositionKeys; ++p) {
         glm::vec3 vec;
         copyVector(channel->mPositionKeys[p].mValue,vec);
         n->positions.emplace_back(vec);
         n->posTimes.emplace_back(channel->mPositionKeys[p].mTime);
         n->numPos = channel->mNumPositionKeys;;
      }

      for (int s = 0; s<channel->mNumScalingKeys; ++s) {
         glm::vec3 vec;
         copyVector(channel->mScalingKeys[s].mValue,vec);
         n->scales.emplace_back(vec);
         n->scaleTimes.emplace_back(channel->mScalingKeys[s].mTime);
         n->numScale = channel->mNumScalingKeys;
      }

      for (int r = 0; r<channel->mNumRotationKeys; ++r) {
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
                   std::vector<std::string> const & names) {

   if (!assimpNode) {
      std::cout << "Node is nullptr \n";
      return;
   }
//   std::cout << "Name:" << assimpNode->mName.C_Str() << std::endl;
//   std::cout << "Nr of Childs:" << assimpNode->mNumChildren << std::endl;
//   std::cout << "Index in con:" << index << std::endl;
   bool skip = false;
   for (std::string bN : names) {
      if (bN.compare(assimpNode->mName.C_Str())!=0) continue;
      skNode.name        = assimpNode->mName.C_Str();
      skNode.numChildren = assimpNode->mNumChildren;
      skNode.boneIndex   = index;
      if (assimpNode->mNumChildren == 0) return;
      for (int i=0; i<assimpNode->mNumChildren; ++i) {
         skNode.children.emplace_back();
         importSkeletonNode(assimpNode->mChildren[i], skNode.children.back(), i, names);
      }
      skip = true;

   }
   if (skip) return;
   for (int i=0; i<assimpNode->mNumChildren; ++i) {
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
