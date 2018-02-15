#include <string>
#include <vector>
#include "RenderAbstraction.hpp"
#include <glm/glm.hpp>

class Mesh {

public:
   void
   loadMesh(std::string path);

   std::vector<VertexN> m_Mesh;
};
