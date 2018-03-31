#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 st;
layout(location = 3) in vec3 binormal;
layout(location = 4) in vec3 tangent;
layout(location = 5) in mat4 world;

layout(std140, row_major, binding =0) uniform UBO
{
	mat4 proj;
	mat4 view;
};

out struct PS
{
	vec3 normal;
	vec2 st;
}ps;

void main()
{
	//Transpose to Row Major
	mat4 unitModel = transpose(world);
	vec4 position = unitModel * vec4(pos,1);
	gl_Position = proj * view * position;
	ps.st = st;
	ps.normal = normal;
};