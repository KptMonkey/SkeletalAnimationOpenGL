#include <string>
#include <vector>
#include <unordered_map>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "RenderAbstraction.hpp"

struct SkeletonNode {
   std::vector<SkeletonNode> children;
   std::string name;
   int         numChildren;
   int         index;
   glm::mat4   relTransformation;

   //Animation relevant stuff :D ;_;
   std::vector<glm::vec3> transformations;
   std::vector<glm::quat> rotations;
   std::vector<glm::vec3> scales;
   std::vector<double>    transTimes;
   std::vector<double>    rotTimes;
   std::vector<double>    scaleTimes;
   int                    numTrans;
   int                    numRot;
   int                    numScale;
};

struct IndexedBoneName {
   std::string name;
   int         index;
};

class Mesh {
public:
   void
   loadMesh(std::string path);


   void
   animateSkeleton(SkeletonNode & node,
                   glm::mat4 const & parentMat,
                   std::vector<glm::mat4> & boneAnimationMat,
                   float animationTime);
private:
   void
   createSkeleton(aiNode * assimpNode, SkeletonNode & skNode);

   void
   createAnimation(aiScene const * scene);

   SkeletonNode *
   findNode(SkeletonNode & node, std::string name);

   std::vector<std::string>             m_AnimatedBones;
   std::unordered_map<std::string,int>  m_BoneIndex;
   float                                m_AnimationDuration;
public:
   SkeletonNode                   m_Skeleton;
   std::vector<unsigned int>      m_Index;
   std::vector<VertexS>           m_Mesh;
   std::vector<glm::mat4>         m_BoneOffSet;
   std::vector<glm::vec3>         m_BonePos;
};
