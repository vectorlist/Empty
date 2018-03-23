#version 450 core
	
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 st;
layout(location = 3) in vec2 uv;
layout(location = 4) in vec3 binormal;
layout(location = 5) in vec3 tangent;

layout(std140, row_major, binding = 0) uniform UBO
{
	mat4 proj;
	mat4 view;
	mat4 model;
};

out struct PS
{
	vec3 normal;
	vec2 st;
}ps;

void main()
{
	gl_Position = proj * view  * vec4(pos,1);
	ps.normal = normal;
	ps.st = st;
}