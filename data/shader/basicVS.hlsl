cbuffer matrices : register(b0)
{
    matrix proj;
    matrix view;
    matrix model;
};

struct VS_In
{
    float3 pos : POSITION; 
    float3 normal : TEXCOORD0;  
};

struct PS_In
{
    float4 pos : SV_POSITION;
    float3 normal : TEXCOORD0;
};

PS_In VS(VS_In vs)
{
    PS_In ps;
    ps.pos = mul(float4(vs.pos,1), view);
    ps.pos = mul(ps.pos, proj);
    ps.normal = vs.normal;
    return ps;
    
};