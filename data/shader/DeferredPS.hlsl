Texture2D gPosition : register(t0);
Texture2D gNormal : register(t1);
Texture2D gAlbedo : register(t2);

SamplerState samplerWarp : register(s0);

struct PS_In
{
	float4 pos : SV_POSITION;
	float2 st : TEXCOORD0;
};

struct Light
{
	float3 pos;
	float3 color;
	float  radius;
};

cbuffer ubo : register(b2)
{
	float3 viewPos;
}

float4 PS(PS_In ps) : SV_TARGET
{
	float3 P = gPosition.Sample(samplerWarp, ps.st).rgb;
	float3 N = gNormal.Sample(samplerWarp, ps.st).rgb;
	float3 albedo = gAlbedo.Sample(samplerWarp, ps.st).rgb;

	float3 lightSum = albedo * 0.1;
	float3 viewDir = normalize(viewPos - P);
	const Light lights[3] = { 
	{ { -12, 5,-12}, {0.9,0.72,0.53}, {3} },
	{ { 2,10,0 },{ 1,0.5,0 },{ 7 } } ,
	{ { -5,4,0 },{ 0.2,0.5,0.9 },{ 6 } } };


	for (int i = 0; i < 3; ++i)
	{
		float distance = length(lights[i].pos - P);

		if (distance < lights[i].radius)
		{
			float3 lightDir = normalize(lights[i].pos - P);
			float3 diffuse = max(dot(N, lightDir), 0) * albedo * lights[i].color;

			float3 halfDir = normalize(lightDir + viewDir);
			float spec = pow(max(dot(N, halfDir), 0), 66);

			float attenuation = 1.0 / (1.0 + 0.0001 * distance + 0.9 * distance * distance);
			diffuse *= attenuation;
			spec *= attenuation;
			lightSum += diffuse;
		}
	}

	return float4(lightSum, 1);
}