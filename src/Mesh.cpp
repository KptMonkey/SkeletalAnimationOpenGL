#include <iostream>
#include "Mesh.hpp"
#include <glm/gtc/matrix_access.hpp>
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
                std::vector<glm::mat4> & globalAnim) {

   static float a = 0.f;
   glm::mat4 localMat(1.f);
   glm::mat4 ourMat = parentMat;
   int boneIdx = node.boneIndex;
   if (boneIdx>-1) {
      glm::mat4 boneOffset = offsetMats[boneIdx];
      glm::mat4 invOffSet  = glm::inverse(boneOffset);
      localMat = globalAnim[boneIdx];
      ourMat = parentMat * invOffSet * localMat * boneOffset;
      boneAnimationMats[boneIdx]=ourMat;
      a +=0.01f;
   }

   for (auto & child : node.children) {
      animateSkeleton(child,ourMat,offsetMats, boneAnimationMats,globalAnim);
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
   importSkeletonNode(root,m_Skeleton,-1,boneName);
   printSkeleton(m_Skeleton);

   if (scene->mNumAnimations <=0) std::cout << "There are no animatoins\n";

}

void
Mesh::importSkeletonNode(aiNode *       assimpNode,
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
      std::cout << "Compare: " << bN << " " << assimpNode->mName.C_Str() << " " << int(bN.compare(assimpNode->mName.C_Str())) << std::endl;
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
