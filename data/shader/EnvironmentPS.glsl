#version 450 core

in PS{
	vec2 st;
}ps;

out vec4 outColor;

uniform sampler2D tex01;

void main()
{
	vec4 texColor = texture(tex01, ps.st);
	vec3 gamma = vec3(1.0 /2.2);
	vec3 color = pow(texColor.rgb, gamma); 
	outColor = vec4(color, texColor.a);
};