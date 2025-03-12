#define MAX_INSTANCE_COUNT 1024

cbuffer vs_buffer_sceneEachFrame : register(b0)
{
    matrix cameraViewProjection;
};

cbuffer vs_buffer_object : register(b1)
{
    matrix world[MAX_INSTANCE_COUNT];
};

struct VS_INPUT
{
    float3 position : POSITION;
    uint instanceID : SV_InstanceID;
};

float4 Main(VS_INPUT input) : SV_POSITION
{
    const matrix wvp = mul(world[input.instanceID], cameraViewProjection);
    return mul(float4(input.position, 1.0f), wvp);
}