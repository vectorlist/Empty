Texture2D tex01 : register(t0);
SamplerState samplerWarp : register(s0);

struct PS_In
{
	float4 pos : SV_POSITION;
	float2 st : TEXCOORD0;
};

float4 PS(PS_In ps) : SV_TARGET
{
	return tex01.Sample(samplerWarp, ps.st);
};