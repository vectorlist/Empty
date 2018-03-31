struct PS_In
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D tex01;
SamplerState samplerState;

float4 PS(PS_In ps) : SV_TARGET
{
	float4 texColor = tex01.Sample(samplerState, ps.uv);
	float3 gamma = 1.0 / 2.2;
	float3 color = pow(texColor, gamma);
	return float4(color, texColor.a);
	//return texColor;
};