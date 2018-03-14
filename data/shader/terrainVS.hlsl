cbuffer matrices : register(b0)
{
    matrix proj;
    matrix view;
    matrix model;
};

struct VS_In
{
    float3 pos : POSITION; 
    float3 normal : TEXCOORD0;  //normal
    float2 st : TEXCOORD1;	//st
    float2 uv : TEXCOORD2;	//uv
    float3 binormal : TEXCOORD3;
    float3 tangent : TEXCOORD4;
};

struct PS_In
{
    float4 pos : SV_POSITION;
    float3 normal : TEXCOORD0;
    float2 st : TEXCOORD1;
    float3 tangent : TEXCOORD2;
};

PS_In VS(VS_In vs)
{
    PS_In ps;
    //vs.pos.w = 1;
    ps.pos = mul(float4(vs.pos,1), view);
    ps.pos = mul(ps.pos, proj);
    ps.st = vs.st;
    ps.normal = vs.normal;
    ps.tangent = vs.tangent;
    
    return ps;
    
};