#version 450 core

out vec4 outColor;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedo;

in PS{
	vec2 st;
}ps;

layout(std140, binding = 2) uniform UBO
{
	vec3 viewPos;
};

struct Light
{
	vec3 pos;
	vec3 color;
};

#define MAX_LIGHT 30

layout(std140, binding = 3) uniform UBO3
{
	Light ligths[MAX_LIGHT];
	int lightNum;
}ubo;

void main()
{
	vec3 P = texture(gPosition, ps.st).rgb;
	vec3 N = texture(gNormal, ps.st).rgb;
	vec3 albedo = texture(gAlbedo, ps.st).rgb;
	//float alpha = texture(gAlbedo, ps.st).a;
	vec3 lightSum = albedo * 0.2;

	//TODO : static Uniform buffer dynamic
	Light lights[3] = {
	{{0,-5,0}, {0.9,0.8,0.4}},
	{{0,10,0},{0.5,0.7,0.9}},
	{{-3,3,3}, {0.1,0.2,0.9}}};

	for(int i =0; i < 2; ++i)
	{
		vec3 lightDir = normalize(lights[i].pos - P);
		vec3 diffuse = max(dot(N, lightDir), 0.0) * albedo * lights[i].color;
		lightSum += diffuse;
	};
	
	outColor = vec4(lightSum, 1);
};