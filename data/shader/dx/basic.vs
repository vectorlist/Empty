
cbuffer Matrix
{
    matrix proj;
    matrix view;
    matrix world;
};

struct VertexInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 st : ST;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float st : ST;
};

//main entry
PixelInput VS(VertexInput input)
{
    PixelInput ps;
    ps.pos.w = 1;
    //m * v * p for hlsl
    
    ps.pos = mul(input.pos, world);
    ps.pos = mul(ps.pos, view);
    ps.pos = mul(ps.pos, proj);
    ps.normal = input.normal;
    ps.st = input.st;

    return ps;
}
