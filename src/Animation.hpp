#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "RenderAbstraction.hpp"

struct Transformation {
   std::vector<glm::vec3> translations;
   std::vector<glm::quat> rotations;
   std::vector<glm::vec3> scales;
   std::vector<double>    transTimes;
   std::vector<double>    rotTimes;
   std::vector<double>    scaleTimes;
   int                    numTrans;
   int                    numRot;
   int                    numScale;
};

struct SkeletonNode {
   std::vector<SkeletonNode> children;
   std::string               name;
   int                       numChildren;
   int                       index;
   glm::mat4                 relTransformation;
   std::unordered_map<std::string, Transformation> animations;
};

struct AnimationData {
   std::vector<std::string> animatedBones;
   float                    animationDuration;
   std::string              name;
};


struct Animation {
   std::unordered_map<std::string, AnimationData>  animationData;
   SkeletonNode                                    skeleton;

};
