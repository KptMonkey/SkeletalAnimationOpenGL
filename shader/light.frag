precision mediump float;
varying vec3 nrm;
varying vec4 worldPos;
uniform vec4 color;
vec3 lightPos = vec3(0.0, 0.0, 30.0);
vec4 lightCol = vec4(1.0, 1.0, 1.0, 10);
void main()
{
  vec3 norm = normalize(nrm);
  vec3 lightDir = normalize(lightPos - vec3(worldPos));
  vec4 amb  = 0.2 * lightCol;
  vec4 diff = max(dot(norm, lightDir), 0.0) * lightCol;
  
  gl_FragColor = (amb + diff) * color;
}
