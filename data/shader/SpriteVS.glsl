#version 450 core

//TODO : INSTANCING

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 st;

layout(std140, row_major, binding = 1) uniform UBO
{
	mat4 screen;
}ubo;

out PS
{
	vec2 st;
}ps;

void main()
{
	gl_Position = ubo.screen * vec4(pos, 1);
	ps.st = st;
};