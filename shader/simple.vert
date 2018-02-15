#version 330 core
layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
uniform mat4 mvp;
out vec3 nrm;
void main()
{
   nrm = normal;
   gl_Position=mvp*vec4(pos,1.0);
}
