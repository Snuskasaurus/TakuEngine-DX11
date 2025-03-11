#define MAX_INSTANCE_COUNT 1024

struct VS_INPUT
{
    float3 position : POSITION;
    uint instanceID : SV_InstanceID;
};

cbuffer cbuffer_frame : register(b0)
{
    matrix cameraViewProjection;
};

cbuffer cbuffer_object : register(b1)
{
    matrix world[MAX_INSTANCE_COUNT];
};

float4 Main(VS_INPUT input) : SV_POSITION
{
    const matrix wvp = mul(world[input.instanceID], cameraViewProjection);
    return mul(float4(input.position, 1.0f), wvp);
}