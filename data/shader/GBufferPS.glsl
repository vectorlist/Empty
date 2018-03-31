#version 450 core

//use glReadBuffers { attachments....}

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbeto;

uniform sampler2D tex01;

in PS{	
	vec3 pos;
	vec3 normal;
	vec2 coords;
}ps;

void main()
{
	gPosition = ps.pos;
	gNormal = normalize(ps.normal);
	gAlbeto = texture(tex01, ps.coords);
};
