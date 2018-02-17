#version 330 core
layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in int bone_id;
uniform mat4 mvp;
uniform mat4 bone_mat[32];
out vec3 nrm;
out vec3 color;
void main()
{
   color = vec3(0.0, 0.0, 0.0);
   if (bone_id==0) {
      color.r = 1.0;
   }
   if (bone_id==1) {
      color.g = 1.0;
   }
   if (bone_id==2) {
      color.b = 1.0;
   }
   nrm = normal;
   gl_Position=mvp*bone_mat[bone_id]*vec4(pos,1.0);
}
