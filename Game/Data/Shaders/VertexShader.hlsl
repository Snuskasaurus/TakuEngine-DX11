#define MAX_INSTANCE_COUNT 1024

struct VS_Input
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tan : TANGENT;
    float2 uv : TEXCOORD;
    uint instanceID : SV_InstanceID;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tan : TANGENT;
    float2 uv : TEXCOORD0;
    float4 positionLight : TEXCOORD1;
};

cbuffer cbuffer_frame : register(b0)
{
    matrix cameraViewProjection;
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
};

cbuffer cbuffer_object : register(b1)
{
    matrix world[MAX_INSTANCE_COUNT];
};

VS_Output Main(VS_Input input)
{
    VS_Output output;

    matrix wvp = mul(world[input.instanceID], cameraViewProjection);

    output.position = mul(float4(input.position, 1.0f), wvp);

    output.positionLight = mul(float4(input.position, 1.0f), world[input.instanceID]);
    output.positionLight = mul(output.positionLight, lightViewMatrix);
    output.positionLight = mul(output.positionLight, lightProjectionMatrix);

    output.normal = mul(input.normal, world[input.instanceID]).rgb;
    output.tan = mul(input.tan, world[input.instanceID]).rgb;

    output.uv = input.uv;
    
    return output;
}