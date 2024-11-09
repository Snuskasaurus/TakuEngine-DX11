struct PS_Input
{
    float2 uv : UV;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

Texture2D texColor : register(t0);
SamplerState samplerState;

float4 main(PS_Input input) : SV_Target
{
    //return float4(input.normal, 1.0f);
    //return float4(input.position);
    //return float4(input.uv, 1.0f, 1.0f);
    return texColor.Sample(samplerState, input.uv);
}