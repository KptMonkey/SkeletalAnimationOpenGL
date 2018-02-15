#version 330 core
out vec4 color;
in vec3 nrm;
void main()
{
  color = vec4(nrm, 1.0);
}
