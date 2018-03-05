#pragma once

#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

struct AnimationVertex {
    glm::vec3  position;
    glm::vec3  normal;
    glm::vec4  weight;
    glm::ivec4 boneId;
};

struct Mesh {
   std::unordered_map<std::string,int> boneIndex;
   std::vector<unsigned int>           indexVec;
   std::vector<AnimationVertex>        mesh;
   std::vector<glm::mat4>              boneOffSet;
   std::vector<glm::vec3>              bonePos;  //Debug Visualization
};
