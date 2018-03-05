#pragma once
#include "Mesh.hpp"
#include "Animation.hpp"
#include <assimp/scene.h>

class AssimpLoader {
public:
   void
   loadMesh(Mesh & mesh, std::string path);

   void
   loadAnimation(Animation & animation, Mesh const & mesh, std::string path);

private:
   aiScene const * m_Scene;
};
