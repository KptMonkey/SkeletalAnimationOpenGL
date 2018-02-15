#version 330 core
layout(location=0) vec3 a_pos;
layout(location=1) vec3 a_nrm;
uniform mat4 mvp;
uniform mat4 model;
out vec3 nrm;
void main()
{
   nrm = a_nrm;
   gl_Position=mvp*vec4(a_pos,1.0);
}
