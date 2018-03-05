#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Animation.hpp"
#include "Mesh.hpp"

class AnimationController {
   public:
   void
   loopAnimation(Animation &              animation,
                 std::string              animationName,
                 std::vector<glm::mat4> & boneAnimationMats,
                 float                    animationTime,
                 Mesh const &             mesh);

   void
   setFirstAnimation(Animation & anim, std::string name) {
      m_LastAnimation = anim.animationData[name];
   }

private:
   void
   animate(SkeletonNode &           node,
           glm::mat4 const &        parentMat,
           std::vector<glm::mat4> & boneAnimationMats,
           float                    animationTime,
           AnimationData &          animationData,
           Mesh const &             mesh,
           std::string              animationName);

   AnimationData m_LastAnimation;
};
