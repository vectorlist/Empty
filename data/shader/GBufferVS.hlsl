cbuffer matrices0 : register(b0)
{
	matrix proj;
	matrix view;
};

cbuffer matrices1 : register(b1)
{
	matrix model;
};

struct VS_In
{
	float3 pos		: POSITION;
	float3 normal	: TEXCOORD0;
	float2 st		: TEXCOORD1;
};

struct PS_In
{
	float4 pos		: SV_POSITION;
	float3 world    : TEXCOORD0;
	float3 normal	: TEXCOORD1;
	float2 st		: TEXCOORD2;
};

float4x4 inverse(float4x4 input)
{
#define minor(a,b,c) determinant(float3x3(input.a, input.b, input.c))
	//determinant(float3x3(input._22_23_23, input._32_33_34, input._42_43_44))

	float4x4 cofactors = float4x4(
		minor(_22_23_24, _32_33_34, _42_43_44),
		-minor(_21_23_24, _31_33_34, _41_43_44),
		minor(_21_22_24, _31_32_34, _41_42_44),
		-minor(_21_22_23, _31_32_33, _41_42_43),

		-minor(_12_13_14, _32_33_34, _42_43_44),
		minor(_11_13_14, _31_33_34, _41_43_44),
		-minor(_11_12_14, _31_32_34, _41_42_44),
		minor(_11_12_13, _31_32_33, _41_42_43),

		minor(_12_13_14, _22_23_24, _42_43_44),
		-minor(_11_13_14, _21_23_24, _41_43_44),
		minor(_11_12_14, _21_22_24, _41_42_44),
		-minor(_11_12_13, _21_22_23, _41_42_43),

		-minor(_12_13_14, _22_23_24, _32_33_34),
		minor(_11_13_14, _21_23_24, _31_33_34),
		-minor(_11_12_14, _21_22_24, _31_32_34),
		minor(_11_12_13, _21_22_23, _31_32_33)
		);
#undef minor
	return transpose(cofactors) / determinant(input);
}

PS_In VS(VS_In vs)
{
	PS_In ps;

	ps.pos = mul(float4(vs.pos, 1), model);
	ps.pos = mul(ps.pos, view);
	ps.pos = mul(ps.pos, proj);
	ps.world = mul(float4(vs.pos, 1), model).xyz;
	ps.st = vs.st;
	//ps.normal = mul(vs.normal, (float3x3)model);

	float4x4 NM = transpose(inverse(model));
	ps.normal = normalize(mul(vs.normal, (float3x3)NM));

	return ps;
}