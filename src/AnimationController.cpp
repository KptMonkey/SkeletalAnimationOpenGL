#include "AnimationController.hpp"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

void
translateNode(float animationTime, Transformation const & trfm, glm::mat4 & transMat) {
   if (trfm.numTrans > 0) {
      int prev = 0;
      int next = 0;
      for (int i=0; i<trfm.numTrans-1; ++i) {
         prev = i;
         next = i+1;
         if (trfm.transTimes[next]>=animationTime) break;
      }

      float total = trfm.transTimes[next]-trfm.transTimes[prev];
      float t     = (animationTime-trfm.transTimes[prev])/total;

      auto pT     = trfm.translations[prev];
      auto nT     = trfm.translations[next];
      auto lerped = pT*(1.f-t) + nT*t;

      transMat = glm::translate(transMat, lerped);
   }
}

void
scaleNode(float animationTime, Transformation const & trfm, glm::mat4 & scaleMat) {
   if (trfm.numScale > 0) {
      int prev = 0;
      int next = 0;
      for (int i=0; i<trfm.numScale-1; ++i) {
         prev = i;
         next = i+1;
         if (trfm.scaleTimes[next]>=animationTime) break;
      }

      float total = trfm.scaleTimes[next]-trfm.scaleTimes[prev];
      float t     = (animationTime-trfm.scaleTimes[prev])/total;

      auto pT     = trfm.scales[prev];
      auto nT     = trfm.scales[next];
      auto lerped = pT*(1.f-t) + nT*t;

      scaleMat = glm::scale(scaleMat, lerped);
   }
}

void
rotateNode(float animationTime, Transformation const & trfm, glm::mat4 & rotMat) {
   if (trfm.numRot > 0) {
      int prev = 0;
      int next = 0;
      for (int i=0; i<trfm.numRot-1; ++i) {
         prev = i;
         next = i+1;
         if (trfm.rotTimes[next]>=animationTime) break;
      }

      float total_t = trfm.rotTimes[next]-trfm.rotTimes[prev];
      float t       = (animationTime-trfm.rotTimes[prev])/total_t;

      auto pR = trfm.rotations[prev];
      auto nR = trfm.rotations[next];
      // Rotation has to be done in positive direction!
      if (glm::dot(pR,nR)<0.f) {
         pR *=-1.f;
      }
      glm::quat q = glm::mix(pR,nR,t);
      rotMat     = glm::toMat4(q);
   }
}

void
AnimationController::loopAnimation(Animation &animation,
                                   std::string animationName,
                                   std::vector<glm::mat4> &boneAnimationMats,
                                   float animationTime,
                                   Mesh const & mesh )
{
   auto animationData = animation.animationData[animationName];
   glm::mat4 one(1.f);
   animate(animation.skeleton, one, boneAnimationMats,
           animationTime, animationData, mesh, animationName);
}
static bool blendi = false;
static float blend = 0.f;

void
AnimationController::animate(SkeletonNode & node,
                             glm::mat4  const & parentMat,
                             std::vector<glm::mat4> & boneAnimationMats,
                             float animationTime,
                             AnimationData & animationData,
                             Mesh const & mesh,
                             std::string animationName) {

   float animationTime2 = animationTime;
   glm::mat4 localMat(1.f);
   glm::mat4 ourMat = parentMat;
   float animationDuration = animationData.animationDuration;
   animationTime = std::fmod(animationTime, animationDuration);
   auto const & trfm = node.animations[animationName];


   float animationDuration2 = m_LastAnimation.animationDuration;
   animationTime2 = std::fmod(animationTime2, animationDuration2);
   auto const & trfm2 = node.animations[m_LastAnimation.name];

   if (m_LastAnimation.name != animationData.name) {
      blendi = true;
   }

   glm::mat4 transMat(1.f);
   translateNode(animationTime, trfm, transMat);
   glm::mat4 scaleMat(1.f);
   scaleNode(animationTime, trfm, scaleMat);
   glm::mat4 rotMat(1.f);
   rotateNode(animationTime, trfm, rotMat);

   glm::mat4 transMat2(1.f);
   translateNode(animationTime2, trfm2, transMat2);
   glm::mat4 scaleMat2(1.f);
   scaleNode(animationTime2, trfm2, scaleMat2);
   glm::mat4 rotMat2(1.f);
   rotateNode(animationTime2, trfm2, rotMat2);

   auto const & animatedBones = animationData.animatedBones;
   auto n = std::find(std::begin(animatedBones), std::end(animatedBones), node.name);

   auto const & animatedBones2 = m_LastAnimation.animatedBones;
   auto n2 = std::find(std::begin(animatedBones2), std::end(animatedBones2), node.name);

   if (n2!=std::end(animatedBones2) && n!=std::end(animatedBones)) {

      if (blendi && node.name == "Pelvis") {
         if (blend > 0.f){
            blend -= 0.05f;
         }
         else {
            blend = 1.00f;
         }
      }
      auto transN = transMat2 * blend + transMat * (1.f-blend);
      auto scaleN = scaleMat2 * blend + scaleMat * (1.f-blend);
      glm::quat q1 = glm::quat_cast(rotMat);
      glm::quat q2 = glm::quat_cast(rotMat2);
      if (glm::dot(q1,q2)<0.f) {
         q1 *=-1.f;
      }
      auto rotN = glm::mix(q1,q2,blend);
      auto rot = glm::toMat4(rotN);

      localMat = transN * scaleN * rot;
      if (blend<0.0f) {
         blend = 0.f;
         blendi = false;
      }
   }
   else if (n!=std::end(animatedBones)) localMat = transMat * rotMat * scaleMat;

   else localMat = node.relTransformation;

   ourMat = parentMat * localMat;
   boneAnimationMats[node.index]=ourMat*mesh.boneOffSet[node.index];

   if (m_LastAnimation.name != animationName && blendi == 0.f) {
      m_LastAnimation = animationData;
   }

   for (auto & child : node.children) {
      animate(child, ourMat, boneAnimationMats, animationTime, animationData, mesh, animationName);
   }
}
