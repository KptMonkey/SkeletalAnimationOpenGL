#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include "RenderAbstraction.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"


int main() {
   RenderContext rctx;
   Camera cam = Camera(glm::vec3(0.0f, 5.0f,30.0f), glm::vec3(0.0f, 1.0f, -1.0f));

   // Shader
   std::string simpleShaderVx = "shader/simple.vert";
   std::string simpleShaderFg = "shader/simple.frag";

   std::string pointShaderVx = "shader/point.vert"; //Debug visualization
   std::string pointShaderFg = "shader/point.frag";

   Shader shader;
   shader.bindShader(simpleShaderVx);
   shader.bindShader(simpleShaderFg);
   Mesh mesh;
   mesh.loadMesh("media/ArmyPilot.x");
   mesh.initIdle();
   VertexArray va;

   va.createIndexBuffer<VertexS>(mesh.m_Mesh, mesh.m_Index);
   va.describeVertexArray(0, 3, GlTypes::Float, 14, GlBool::False, 0);
   va.describeVertexArray(1, 3, GlTypes::Float, 14, GlBool::False, 3);
   va.describeVertexArray(2, 4, GlTypes::Float, 14, GlBool::False, 6);
   va.describeVertexArray(3, 4, GlTypes::Int,   14, GlBool::False, 10);

   Shader pointShader;
   pointShader.bindShader(pointShaderVx);
   pointShader.bindShader(pointShaderFg);
   VertexArray p;
   p.createVertexArray<glm::vec3>(mesh.m_BonePos);
   p.describeVertexArray(0,3,GlTypes::Float, 3, GlBool::False,0);
   glm::mat4 model(1.f);

   //Sucks though
   std::vector<glm::mat4> animations;
   for ( int i = 0; i < mesh.m_BoneOffSet.size(); ++i) {
      animations.emplace_back(1.f);
   }
   auto start = std::chrono::steady_clock::now();
   model = glm::scale(model,glm::vec3(0.08f));
   bool running = true;
   std::string anim = "Idle";
   bool blend = false;
   while(running) {
      const Uint8 *state = SDL_GetKeyboardState(nullptr);
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
         switch (e.type) {

         case SDL_QUIT:
            running = false;
            break;

         case SDL_KEYDOWN:
            if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
               running = false;
               break;
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_W) {
               model = glm::translate(model, glm::vec3(0.f,0.f,-1.0f));
               anim = "Run_Forwards";
               break;
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_S) {
               model = glm::translate(model, glm::vec3(0.f,0.f,1.0f));
               anim = "Walk_Backwards";
               break;
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_A) {
               model = glm::translate(model, glm::vec3(-1.f,0.f,0.0f));
               anim = "Strafe_Right";
               break;
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_D) {
               model = glm::translate(model, glm::vec3(1.f,0.f,0.0f));
               anim = "Strafe_Left";
               break;
            }
         case SDL_KEYUP:

            anim = "Idle";
            std::cout << "keyup \n";
            break;

         default: break;
         }
      }
      auto end = std::chrono::steady_clock::now();
      auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end-start ).count();
      glm::mat4 i(1.f);
      mesh.animate(mesh.m_Skeleton, i, animations, dur/10.f, anim);

      glViewport(0,0,800,600);
//      p.bindVBO(mesh.m_BonePos);
      rctx.enableDepthTest();
      rctx.clearColor(0.0f, 0.0f, 0.1f, 1.0f);
      rctx.clearColorBuffer();
      rctx.clearDepthBuffer();


      shader.activate();
      shader["mvp"] = cam.Projection * cam.View * model;
      for (int i = 0; i<mesh.m_BoneOffSet.size(); ++i) {
         std::stringstream ss;
         ss <<"bone_mat[";
         ss << i;
         ss << "]";
         shader[ss.str()]=animations[i];
      }

      va.bindVertexArray();
      rctx.drawIndex(PrimitiveType::Triangle, mesh.m_Index.size() );
//      // Debug Visualization of bone positions
//      rctx.disableDepthTest();
//      pointShader.activate();
//      pointShader["mvp"] = cam.Projection * cam.View * model;
//      p.bindVertexArray();
//      glEnable(GL_PROGRAM_POINT_SIZE);
//      rctx.draw(p,PrimitiveType::Points);
      rctx.swapBuffers();
   }
}
