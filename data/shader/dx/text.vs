cbuffer Matrix
{
    matrix proj;
    matrix view;
    matrix world;
};

struct VSInput
{
    float4 pos : POSITION;
    float2 st : ST;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 st : ST;
};

PSInput VS(VSInput input)
{
    PSInput p;
    input.pos.w = 1;

    //we use ortho for projection
    p.pos = mul(input.pos, proj);
    p.st = input.st;

    return p;
};