#version 330 core
out vec4 col;
in vec3 nrm;
in vec3 color;
void main()
{
  col = vec4(nrm,1.0);
}
