#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include "RenderAbstraction.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"


int main() {
   std::vector<int> bones;
   RenderContext rctx;
   Camera cam = Camera(glm::vec3(0.0f, 0.0f,15.0f), glm::vec3(0.0f, 1.0f, -1.0f));
   double animationDuration = 0.0; // Would be a container for more animations

   // Shader
   std::string simpleShaderVx = "shader/simple.vert";
   std::string simpleShaderFg = "shader/simple.frag";

   std::string pointShaderVx = "shader/point.vert"; //Debug visualization
   std::string pointShaderFg = "shader/point.frag";

   Shader shader;
   shader.bindShader(simpleShaderVx);
   shader.bindShader(simpleShaderFg);
   Mesh mesh;
   mesh.loadMesh("media/ArmyPilot.dae", bones, animationDuration);
   VertexArray va;

   va.createVertexArray(mesh.m_Mesh);
   va.describeVertexArray(0,3,GlTypes::Float, 6, GlBool::False,0);
   va.describeVertexArray(1,3,GlTypes::Float, 6, GlBool::False,3);
   va.bindVBO(mesh.m_BoneIds);
   va.bindVBO(mesh.m_Weights);

   Shader pointShader;
   pointShader.bindShader(pointShaderVx);
   pointShader.bindShader(pointShaderFg);
   VertexArray p;
   p.createVertexArray(mesh.m_BonePos);
   p.describeVertexArray(0,3,GlTypes::Float, 3, GlBool::False,0);
   glm::mat4 model(1.f);

   //Sucks though
   std::vector<glm::mat4> animations;
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);

   auto start = std::chrono::steady_clock::now();
   model = glm::scale(model,glm::vec3(0.04f));
   int fps = 0;
   while(fps<=500) {
      glViewport(0,0,800,600);
//      p.bindVBO(mesh.m_BonePos);
      rctx.enableDepthTest();
      rctx.clearColor(0.0f, 0.0f, 0.1f, 1.0f);
      rctx.clearColorBuffer();
      rctx.clearDepthBuffer();
      glm::mat4 i(1.f);
      auto end = std::chrono::steady_clock::now();
      auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end-start ).count();
//      mesh.animateSkeleton(mesh.m_Skeleton,i ,mesh.m_BoneOffSet,animations, dur/1000.f);

      shader.activate();
      shader["mvp"] = cam.Projection * cam.View * model;
      for (int i = 0; i<3; ++i) {
         std::stringstream ss;
         ss <<"bone_mat[";
         ss << i;
         ss << "]";
         shader[ss.str()]=animations[i];
      }


      va.bindVertexArray();
      rctx.draw(va, PrimitiveType::Triangles);
      // Debug Visualization of bone positions
      rctx.disableDepthTest();
      pointShader.activate();
      pointShader["mvp"] = cam.Projection * cam.View * model;
      p.bindVertexArray();
      glEnable(GL_PROGRAM_POINT_SIZE);
      rctx.draw(p,PrimitiveType::Points);
      rctx.swapBuffers();
      ++fps;
   }
}
