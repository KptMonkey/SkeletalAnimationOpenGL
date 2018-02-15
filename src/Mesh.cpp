#include <iostream>
#include "Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
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
Mesh::loadMesh(std::string path) {
   Assimp::Importer importer;
   auto scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate);

   if (!scene)              std::cout << "Could not load scene! \n";
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
}
