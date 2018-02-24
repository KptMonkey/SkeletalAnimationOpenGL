#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include "RenderAbstraction.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"


int main() {
   RenderContext rctx;
   Camera cam = Camera(glm::vec3(0.0f, 0.0f,30.0f), glm::vec3(0.0f, 1.0f, -1.0f));

   // Shader
   std::string simpleShaderVx = "shader/simple.vert";
   std::string simpleShaderFg = "shader/simple.frag";

   std::string pointShaderVx = "shader/point.vert"; //Debug visualization
   std::string pointShaderFg = "shader/point.frag";

   Shader shader;
   shader.bindShader(simpleShaderVx);
   shader.bindShader(simpleShaderFg);
   Mesh mesh;
   mesh.loadMesh("media/ArmyPilot.dae");
   VertexArray va;

   va.createIndexBuffer(mesh.m_Mesh, mesh.m_Index);
   va.describeVertexArray(0,3,GlTypes::Float, 14, GlBool::False,0);
   va.describeVertexArray(1,3,GlTypes::Float, 14, GlBool::False,3);
   va.describeVertexArray(2,4,GlTypes::Float, 14, GlBool::False,6);
   va.describeVertexArray(3,4,GlTypes::Int, 14, GlBool::False,10);

   Shader pointShader;
   pointShader.bindShader(pointShaderVx);
   pointShader.bindShader(pointShaderFg);
   VertexArray p;
   p.createVertexArray(mesh.m_BonePos);
   p.describeVertexArray(0,3,GlTypes::Float, 3, GlBool::False,0);
   glm::mat4 model(1.f);

   //Sucks though
   std::vector<glm::mat4> animations;
   for ( int i = 0; i < mesh.m_BoneOffSet.size(); ++i) {
      animations.emplace_back(1.f);
   }
   auto start = std::chrono::steady_clock::now();
   model = glm::scale(model,glm::vec3(0.08f));
   int fps = 0;
   while(fps<=700) {
      glViewport(0,0,800,600);
//      p.bindVBO(mesh.m_BonePos);
      rctx.enableDepthTest();
      rctx.clearColor(0.0f, 0.0f, 0.1f, 1.0f);
      rctx.clearColorBuffer();
      rctx.clearDepthBuffer();

      auto end = std::chrono::steady_clock::now();
      auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end-start ).count();

      glm::mat4 i(1.f);
      mesh.animateSkeleton(mesh.m_Skeleton, i, animations, dur/1000.f);
      model = glm::rotate(model,0.01f,glm::vec3(0.f,1.f, 0.f));

      shader.activate();
      shader["mvp"] = cam.Projection * cam.View * model;
      for (int i = 0; i<68; ++i) {
         std::stringstream ss;
         ss <<"bone_mat[";
         ss << i;
         ss << "]";
         shader[ss.str()]=animations[i];
      }

      va.bindVertexArray();
      rctx.drawIndex(PrimitiveType::Triangles, mesh.m_Index.size() );
//      // Debug Visualization of bone positions
//      rctx.disableDepthTest();
//      pointShader.activate();
//      pointShader["mvp"] = cam.Projection * cam.View * model;
//      p.bindVertexArray();
//      glEnable(GL_PROGRAM_POINT_SIZE);
//      rctx.draw(p,PrimitiveType::Points);
      rctx.swapBuffers();
      ++fps;
   }
}
