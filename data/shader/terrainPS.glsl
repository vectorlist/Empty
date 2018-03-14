#version 450 core
out vec4 outCol;

in struct PS
{
	vec3 normal;
	vec2 st;
}ps;

void main()
{
	
	outCol = vec4(ps.st,0,1);
};