#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 st;
layout(location = 3) in vec3 binormal;
layout(location = 4) in vec3 tangent;
//INSTANCING

layout(std140, row_major, binding = 0) uniform UBO0
{
	mat4 proj;
	mat4 view;
};

layout(std140, row_major, binding = 1) uniform UBO1
{
	mat4 model;
};

out struct PS
{
	vec3 normal;
	vec2 st;
}ps;

void main()
{
	gl_Position = proj * view * model * vec4(pos, 1);
	ps.st = st;
};