#include <iostream>
#include <vector>
#include <sstream>
#include "RenderAbstraction.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"


int main() {
   std::vector<int> bones;
   RenderContext rctx;
   Camera cam = Camera(glm::vec3(0.0f, 0.0f,15.0f), glm::vec3(0.0f, 1.0f, -1.0f));
   double animationDuration = 0.0; // Would be a container for more animations

   std::vector<glm::mat4> s_rotations;
   s_rotations.emplace_back(1.f);
   s_rotations.emplace_back(1.f);
   s_rotations.emplace_back(1.f);
   s_rotations[0]=glm::rotate(s_rotations[0],0.8f,glm::vec3(1.f,0.f,0.f));

   // Shader
   std::string simpleShaderVx = "shader/simple.vert";
   std::string simpleShaderFg = "shader/simple.frag";

   std::string pointShaderVx = "shader/point.vert"; //Debug visualization
   std::string pointShaderFg = "shader/point.frag";

   std::vector<glm::mat4> animMats;
   float y = 0.0f;
   Shader shader;
   shader.bindShader(simpleShaderVx);
   shader.bindShader(simpleShaderFg);
   Mesh mesh;
   mesh.loadMesh("media/monkeyS.dae", bones, animationDuration);
   VertexArray va;

   va.createVertexArray(mesh.m_Mesh);
   va.describeVertexArray(0,3,GlTypes::Float, 6, GlBool::False,0);
   va.describeVertexArray(1,3,GlTypes::Float, 6, GlBool::False,3);
   va.bindVBO(bones);

   Shader pointShader;
   pointShader.bindShader(pointShaderVx);
   pointShader.bindShader(pointShaderFg);
   VertexArray p;
   p.createVertexArray(mesh.m_BonePos);
   p.describeVertexArray(0,3,GlTypes::Float, 3, GlBool::False,0);
   glm::mat4 model(1.f);
   glm::mat4 r(1.f);
   std::vector<glm::mat4> animations;
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   animations.emplace_back(1.f);
   glm::mat4 k(1.f);

   int fps = 0;
   while(fps<=400) {
      glViewport(0,0,800,600);
      rctx.enableDepthTest();
      rctx.clearColor(0.0f, 0.0f, 0.1f, 1.0f);
      rctx.clearColorBuffer();
      rctx.clearDepthBuffer();
      mesh.animateSkeleton(mesh.m_Skeleton,animations[2],mesh.m_BoneOffSet,animations,s_rotations);
      animations[2] = glm::rotate(animations[2],0.04f,glm::vec3(1.f, 0.f, 0.f));

      shader.activate();
      shader["mvp"] = cam.Projection * cam.View * model;
      for (int i = 0; i<3; ++i) {
         std::stringstream ss;
         ss <<"bone_mat[";
         ss << i;
         ss << "]";
//         glm::mat4 m(1.f);
//         if (i==1) {
//            r = glm::rotate(r,0.04f,glm::vec3(1.f, 0.f, 0.f));
//            shader[ss.str()]=glm::inverse(mesh.m_BoneOffSet[i])*r*mesh.m_BoneOffSet[i];
//         }
//         else {
//            shader[ss.str()]=m;
//         }
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
