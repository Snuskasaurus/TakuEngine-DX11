cbuffer c_buffer : register(b0)
{
    matrix WORLD_VIEW_PROJECTION;
};

struct VS_Input
{
    float3 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct VS_Output
{
    float2 uv : UV;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

VS_Output main(VS_Input input)
{
    VS_Output output;

    output.position = mul(float4(input.position, 1.0f), WORLD_VIEW_PROJECTION);
    //output.normal = mul(float3(input.normal), TRANSFORM);
    output.normal = input.normal;
    
    output.uv = input.uv;

    return output;
}