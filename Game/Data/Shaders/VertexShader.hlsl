#define MAX_INSTANCE_COUNT 512

struct VS_Input
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint instanceID : SV_InstanceID;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct instancedObject
{
    matrix wvp;
    matrix world;
};

cbuffer cbuffer_object : register(b0)
{
    instancedObject instances[MAX_INSTANCE_COUNT];
};

VS_Output Main(VS_Input input)
{
    VS_Output output;
    
    output.uv = input.uv;
    output.position = mul(float4(input.position, 1.0f), instances[input.instanceID].wvp);
    output.normal = mul(input.normal, instances[input.instanceID].world).rgb;
    
    return output;
}