cbuffer matrices : register(b0)
{
	matrix proj;
	matrix view;
	matrix model;		//not used
};

struct VS_In
{
	float3 pos : POSITION;
	float2 st  : TEXCOORD0;
};

struct PS_In
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

PS_In VS(VS_In vs)
{
	PS_In ps;

	//TEST
	//Remove Translate
	matrix viewLocal = view;
	viewLocal._41 = 0;
	viewLocal._42 = 0;
	viewLocal._43 = 0;
	
	ps.pos = mul(float4(vs.pos, 1), viewLocal);
	ps.pos = mul(ps.pos, proj);

	//make sure Cube max z to NDC max depth
	//Disable DepthStencil on outside
	ps.pos.z = ps.pos.w;
	ps.uv = vs.st;

	return ps;
}

