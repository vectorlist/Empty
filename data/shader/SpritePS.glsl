#version 450 core

//TODO : Texture
layout(location = 0) uniform sampler2D tex01;

in PS
{
	vec2 st;
}ps;
//TODO : Optional Texture or just round color

out vec4 outColor;

void main()
{
	//ps.st.y = 1.0 - ps.st.y;
	vec2 uv = vec2(ps.st.x,1.0 -ps.st.y);
	vec4 texColor = texture(tex01, uv);
	outColor = texColor;
};