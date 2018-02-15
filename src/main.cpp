#include <iostream>
#include <vector>
#include "RenderAbstraction.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"


int main() {
   RenderContext rctx;
   Camera cam = Camera(glm::vec3(0.0f, 0.0f,15.0f), glm::vec3(0.0f, 1.0f, -1.0f));

   // Shader
   std::string simpleShaderVx = "shader/simple.vert";
   std::string simpleShaderFg = "shader/simple.frag";

   Shader shader;
   shader.bindShader(simpleShaderVx);
   shader.bindShader(simpleShaderFg);
   Mesh mesh;
   mesh.loadMesh("media/test.dae");
   VertexArray va;

   va.createVertexArray(mesh.m_Mesh);
   va.describeVertexArray(0,3,GlTypes::Float, 6, GlBool::False,0);
   va.describeVertexArray(1,3,GlTypes::Float, 6, GlBool::False,3);
   glm::mat4 model(1.f);
   while(true) {
      glViewport(0,0,800,600);
      rctx.enableDepthTest();
      rctx.clearColor(0.0f, 0.0f, 0.1f, 1.0f);
      rctx.clearColorBuffer();
      rctx.clearDepthBuffer();
      model = glm::rotate(model, 0.05f,glm::vec3(1.f,0.f,0.f));
      shader.activate();
      shader["mvp"] = cam.Projection * cam.View * model;
      va.bindVertexArray();
      rctx.draw(va, PrimitiveType::Triangles);
      rctx.swapBuffers();
   }
}
