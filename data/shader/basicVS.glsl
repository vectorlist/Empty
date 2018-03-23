#version 450 core
	
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

layout(std140, row_major, binding = 0) uniform UBO
{
	mat4 proj;
	mat4 view;
	mat4 model;
};

out struct PS{
    vec3 normal;
}ps;

void main()
{
	gl_Position = proj * view  * vec4(pos,1);
	ps.normal = normal;
}