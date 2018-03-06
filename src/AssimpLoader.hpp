#pragma once
#include "Mesh.hpp"
#include "Animation.hpp"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

class AssimpLoader {
public:
   void
   loadMesh(Mesh & mesh, std::string path);

   void
   loadAnimation(Animation & animation, Mesh const & mesh);

private:
   Assimp::Importer m_Importer;
   aiScene const * m_Scene;
};
