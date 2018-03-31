texture2D diffTex;
SamplerState sampler2d;

struct PS_In
{
	float4 pos : SV_POSITION;
	float3 normal : TEXCOORD0;
	float2 st : TEXCOORD1;
};

float4 PS(PS_In ps) : SV_TARGET
{
	float4 texColor = diffTex.Sample(sampler2d, ps.st);

	if (texColor.a < 0.5)
		clip(-1);		//same as glsl discard

	return texColor;
}