#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 st;

layout(std140, row_major, binding = 0) uniform UBO0
{
	mat4 proj;
	mat4 view;
};

layout(std140, row_major, binding = 1) uniform UBO1
{
	mat4 model;
};

out PS{	
	vec3 pos;
	vec3 normal;
	vec2 coords;
}ps;

void main()
{
	vec4 world = model * vec4(pos, 1);
	ps.pos = world.xyz;
	ps.coords = st;
	
	mat3 NM = transpose(inverse(mat3(model)));
	
	ps.normal = NM * normal;	

	gl_Position = proj * view * world;
};