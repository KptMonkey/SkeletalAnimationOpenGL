#version 330 core
layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec4 weigth;
layout(location=3) in ivec4 bone_id;
uniform mat4 mvp;
uniform mat4 bone_mat[100];
out vec3 nrm;
out vec3 color;
void main()
{
   nrm = vec3(1.0);
   if ( (bone_id.x==18 && weigth.x > 0.5 ) ||
	(bone_id.y==18 && weigth.y > 0.5 ) ||
	(bone_id.z==18 && weigth.z > 0.5 ) ||
	(bone_id.w==18 && weigth.w > 0.5 ) )
	{
		nrm = vec3(1.0,0.0,0.0);
	}
   nrm = normal;
   mat4 trans = bone_mat[bone_id[0]] * weigth[0];
   trans     += bone_mat[bone_id[1]] * weigth[1];
   trans     += bone_mat[bone_id[2]] * weigth[2];
   trans     += bone_mat[bone_id[3]] * weigth[3];

   gl_Position=mvp*trans*vec4(pos,1.0);
}
