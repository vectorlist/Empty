#version 450 core
//flat quad
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 st;

out PS{
	vec2 st;
}ps;

void main()
{
	ps.st = st;
	ps.st.y = 1.0 - st.y;		//match hlsl
	gl_Position = vec4(pos, 1);
};