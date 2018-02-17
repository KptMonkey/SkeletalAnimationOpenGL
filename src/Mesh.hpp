#include <string>
#include <vector>
#include <assimp/scene.h>
#include <array>
#include "RenderAbstraction.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct SkeletonNode {
   std::vector<SkeletonNode> children;
   std::string name;
   int numChildren;
   int boneIndex;

   //Animation relevant stuff :D ;_;
   std::vector<glm::vec3> positions;
   std::vector<glm::quat> rotations;
   std::vector<glm::vec3> scales;
   std::vector<double>    posTimes;
   std::vector<double>    rotTimes;
   std::vector<double>    scaleTimes;
   int                    numPos;
   int                    numRot;
   int                    numScale;
};

class Mesh {
public:
   void
   loadMesh(std::string path, std::vector<int> & boneIds, double & animationDuration);

   void
   importSkeletonNode(aiNode * assimpNode, SkeletonNode & skNode,
                      int index, std::vector<std::string> const & boneName);

   void
   animateSkeleton(SkeletonNode & node,
                   glm::mat4 & parentMat,
                   std::vector<glm::mat4> & offsetMats,
                   std::vector<glm::mat4> & boneAnimationMat,
                   std::vector<glm::mat4> & globalAnim,
                   float animationTime);


   SkeletonNode *
   findNode(SkeletonNode & node, std::string name);

   std::vector<VertexN>     m_Mesh;
   std::vector<glm::mat4>   m_BoneOffSet;
   std::vector<glm::vec3>   m_BonePos;
   std::vector<std::string> boneName;
   SkeletonNode             m_Skeleton;
};
