#define MAX_INSTANCE_COUNT 1024

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
    matrix world;
};

cbuffer cbuffer_frame : register(b0)
{
    matrix cameraViewProjection;
};

cbuffer cbuffer_object : register(b1)
{
    instancedObject instances[MAX_INSTANCE_COUNT];
};

VS_Output Main(VS_Input input)
{
    VS_Output output;

    matrix wvp = mul(instances[input.instanceID].world, cameraViewProjection);
    
    output.uv = input.uv;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.normal = mul(input.normal, instances[input.instanceID].world).rgb;
    
    return output;
}