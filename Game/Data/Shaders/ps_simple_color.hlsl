struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : TEXCOORD;
};

float4 Main(VS_OUTPUT input) : SV_Target
{
    return input.color;
}