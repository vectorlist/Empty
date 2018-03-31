struct VS_In
{
	float3 pos : POSITION;
	float2 st : TEXCOORD0;
};

struct PS_In
{
	float4 pos : SV_POSITION;
	float2 st : TEXCOORD0;
};

PS_In VS(VS_In vs)
{
	PS_In ps;

	ps.pos = float4(vs.pos, 1);
	ps.st = vs.st;
	//ps.st.y = 1.0 - vs.st.y;

	return ps;
};