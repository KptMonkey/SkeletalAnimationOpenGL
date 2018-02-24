# Animation
Loads and plays skeletal animations. Until now .dae and .x is supported
```
Mesh mesh;
mesh.loadMesh(path)
std::vector<glm::mat4> boneAnimations;
for (int i=0; i<mesh.BoneOffSet.size(); ++i) {
  boneAnimations.emplace_back(1.f);
}
auto start = std::chrono::steady_clock::now();

while(render) {
  auto end = std::chrono::steady_clock::now();
  auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end-start ).count();

  glm::mat4 i(1.f);
  mesh.animateSkeleton(mesh.m_Skeleton, i, animations, dur/1000.f);

  //Init shader with bone animations for example like that
  for (int i = 0; i<mesh.m_BoneOffSet.size(); ++i) {
    std::stringstream ss;
    ss <<"bone_mat[";
    ss << i;
    ss << "]";
    shader[ss.str()]=animations[i];
  }
  // Draw mesh
}

```
