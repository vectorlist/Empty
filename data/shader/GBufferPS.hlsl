Texture2D tex01 : register(t0);
SamplerState samplerWarp : register(s0);

struct PS_In
{
	float4 pos		: SV_POSITION;
	float3 world    : TEXCOORD0;
	float3 normal	: TEXCOORD1;
	float2 st		: TEXCOORD2;
};

//Output Type
struct PS_Out
{
	float4 pos : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 albedo : SV_TARGET2;
};


PS_Out PS(PS_In ps) : SV_TARGET
{
	PS_Out output;
	output.pos = float4(ps.world, 1);
	output.normal = float4(ps.normal, 1);
	output.albedo = tex01.Sample(samplerWarp, ps.st);

	return output;

}
