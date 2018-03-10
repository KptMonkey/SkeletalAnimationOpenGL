# Animation
Loads and plays skeletal animations with smooth transitions(Simple blending). Until now .dae and .x is supported. 
Move the character with W,A,S,D

ToDo:
- Non loop animations (e.g Jump)
- IK 
- IK blends with current animation
- RagDoll
- Animation Compression based on https://engineering.riotgames.com/news/compressing-skeletal-animation-data


Example Code:
```
int main() {
   RenderContext rctx;
   Camera cam = Camera(glm::vec3(0.0f, 5.0f,30.0f), glm::vec3(0.0f, 1.0f, -1.0f));
   KeyboardHandler inputHandler;
   // Shader
   std::string simpleShaderVx = "shader/simple.vert";
   std::string simpleShaderFg = "shader/simple.frag";

   Shader shader;
   shader.bindShader(simpleShaderVx);
   shader.bindShader(simpleShaderFg);

   Mesh      soldierMesh;
   Animation soldierAnimation;

   AssimpLoader loader;
   loader.loadMesh(soldierMesh, "media/ArmyPilot.x");
   loader.loadAnimation(soldierAnimation, soldierMesh, "media/ArmyPilot.x");

   AnimationController animationCtrl;
   animationCtrl.setFirstAnimation(soldierAnimation,"Idle");

   VertexArray va;
   va.createIndexBuffer<AnimationVertex>(soldierMesh.mesh, soldierMesh.indexVec);
   va.describeVertexArray(0, 3, GlTypes::Float, 14, GlBool::False, 0);
   va.describeVertexArray(1, 3, GlTypes::Float, 14, GlBool::False, 3);
   va.describeVertexArray(2, 4, GlTypes::Float, 14, GlBool::False, 6);
   va.describeVertexArray(3, 4, GlTypes::Int,   14, GlBool::False, 10);


   //Sucks though
   std::vector<glm::mat4> animations;
   for ( int i = 0; i < soldierMesh.boneOffSet.size(); ++i) {
      animations.emplace_back(1.f);
   }

   glm::mat4 model(1.f);
   model            = glm::scale(model,glm::vec3(0.08f));

   bool running     = true;
   std::string anim = "Idle";

   auto start = std::chrono::steady_clock::now();
   while(running) {
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
         switch (e.type) {

         case SDL_QUIT:
            inputHandler.handleKeyboardEvent(e.key);
            break;
         case SDL_KEYDOWN:
            inputHandler.handleKeyboardEvent(e.key);
            break;
         case SDL_KEYUP:
            inputHandler.handleKeyboardEvent(e.key);
            anim = "Idle";
            break;

         default: break;
         }
      }

      if (inputHandler.isPressed(SDL_SCANCODE_W)) {
         model = glm::translate(model, glm::vec3(0.f,0.f,-1.0f));
         anim = "Run_Forwards";
      }
      if (inputHandler.isPressed(SDL_SCANCODE_S)) {
         model = glm::translate(model, glm::vec3(0.f,0.f,1.0f));
         anim = "Run_backwards";
      }
      if (inputHandler.isPressed(SDL_SCANCODE_A)) {
         model = glm::translate(model, glm::vec3(-1.f,0.f,0.0f));
         anim = "Strafe_Right";
      }
      if (inputHandler.isPressed(SDL_SCANCODE_D)) {
         model = glm::translate(model, glm::vec3(1.f,0.f,0.0f));
         anim = "Strafe_Left";
      }
      if (inputHandler.isPressed(SDL_SCANCODE_ESCAPE)) {
         running = false;
      }

      auto end = std::chrono::steady_clock::now();
      auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end-start ).count();

      animationCtrl.loopAnimation(soldierAnimation, anim, animations, dur/10.f, soldierMesh);

      glViewport(0,0,800,600);
      rctx.enableDepthTest();
      rctx.clearColor(0.0f, 0.0f, 0.1f, 1.0f);
      rctx.clearColorBuffer();
      rctx.clearDepthBuffer();


      shader.activate();
      shader["mvp"] = cam.Projection * cam.View * model;
      for (int i = 0; i<soldierMesh.boneOffSet.size(); ++i) {
         std::stringstream ss;
         ss <<"bone_mat[";
         ss << i;
         ss << "]";
         shader[ss.str()]=animations[i];
      }

      va.bindVertexArray();
      rctx.drawIndex(PrimitiveType::Triangle, soldierMesh.indexVec.size() );
      rctx.swapBuffers();
   }
}

```
