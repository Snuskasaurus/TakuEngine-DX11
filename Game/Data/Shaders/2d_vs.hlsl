struct VS_INPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_OUTPUT Main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = input.pos;
    output.uv = input.uv;
    return output;
}