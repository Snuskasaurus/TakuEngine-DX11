#define MAX_INSTANCE_COUNT 1024

cbuffer cbuffer_object : register(b0)
{
    matrix object_wvp[MAX_INSTANCE_COUNT];
};

struct VS_Input
{
    float3 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    uint instanceID : SV_InstanceID;
};

struct VS_Output
{
    float2 uv : UV;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

VS_Output Main(VS_Input input)
{
    VS_Output output;

    output.position = mul(float4(input.position, 1.0f), object_wvp[input.instanceID]);
    //output.normal = mul(float3(input.normal), TRANSFORM);
    output.normal = input.normal;
    
    output.uv = input.uv;

    return output;
}