#version 450 core

in PS{
	vec2 st;
}ps;

out vec4 outColor;

uniform sampler2D tex01;

void main()
{
	vec4 texColor = texture(tex01, ps.st);
	outColor = texColor;
};