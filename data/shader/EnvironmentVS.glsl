#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 st;

layout(std140, row_major, binding = 0) uniform UBO
{
	mat4 proj;
	mat4 view;
};

out PS{
	vec2 st;
}ps;

void main()
{
	//column major rule in glsl whatever input major
	//remove translate
	mat4 viewLocal = view;
	viewLocal[3][0] = 0; 
	viewLocal[3][1] = 0;
	viewLocal[3][2] = 0; // [3][3] must be 1.0

	vec4 P = proj*  viewLocal * vec4(pos,1);
	//disable Depth DepthStencil test
	//limit z valur to NDC max depth
	P.z = P.w;
	gl_Position = P;
	ps.st = st;

	//P.z = 1.0 depth other are must be 0.0 to 1.0 (so we need equal and less)
};