cbuffer matrices : register(b0)
{
	matrix proj;
	matrix view;
};

cbuffer matrices0 : register(b1)
{
	matrix model;
};

struct VS_In
{
	float3 pos : POSITION;
	float3 normal : TEXCOORD0;
	float2 st : TEXCOORD1;
	float3 binormal : TEXCOORD2;
	float3 tangent : TEXCOORD3;
};

struct PS_In
{
	float4 pos : SV_POSITION;
	float3 normal : TEXCOORD0;
	float2 st : TEXCOORD1;
};

PS_In VS(VS_In vs)
{
	PS_In ps;

	ps.pos = mul(float4(vs.pos, 1), model);
	ps.pos = mul(ps.pos, view);
	ps.pos = mul(ps.pos, proj);
	ps.normal = vs.normal;
	ps.st = vs.st;

	return ps;
};