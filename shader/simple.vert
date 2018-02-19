#version 330 core
layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in ivec3 bone_id;
layout(location=3) in vec3 weigth;
uniform mat4 mvp;
uniform mat4 bone_mat[100];
out vec3 nrm;
out vec3 color;
void main()
{
   color = weigth;
   nrm = normal;
   mat4 trans = bone_mat[bone_id.x] * weigth.x;
   trans += bone_mat[bone_id[1]] * weigth[1];
   trans += bone_mat[bone_id[2]] * weigth[2];

   gl_Position=mvp*trans*vec4(pos,1.0);
}
