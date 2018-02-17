#version 330 core
out vec4 col;
in vec3 nrm;
in vec3 color;
void main()
{
  col = vec4(color, 1.0);
}
