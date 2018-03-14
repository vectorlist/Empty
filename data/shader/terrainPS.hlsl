struct PS_In
{
    float4 pos : SV_POSITION;
    float3 normal : TEXCOORD0;
    float2 st : TEXCOORD1;
    float3 tangent : TEXCOORD2;
};

float4 PS(PS_In ps) : SV_TARGET
{
    float3 diffuse = float3(1,1,1);
    float3 lightDir = float3(3,3,3) - float3(0,0,0);
    float dist = length(lightDir);

    float delta = dot(ps.normal, normalize(lightDir));
    delta = (delta + 1) * 0.5;

    float interval = 1.0 / 4.0;
    
    float toon = 0;
    toon = floor(delta/ interval) * interval;
    toon = max(0.3, toon);

    //diffuse = toon * diffuse;
    
    return float4(ps.tangent * delta,1);
};