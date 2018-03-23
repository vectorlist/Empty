struct PS_In
{
    float4 pos : SV_POSITION;
    float3 normal : TEXCOORD0;
};

float4 PS(PS_In ps) : SV_TARGET
{

    return float4(ps.normal,1);
};