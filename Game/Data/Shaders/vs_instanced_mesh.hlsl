#define MAX_INSTANCE_COUNT 1024

cbuffer vs_buffer_sceneEachFrame : register(b0)
{
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
};

cbuffer vs_buffer_object : register(b1)
{
    matrix world[MAX_INSTANCE_COUNT];
};

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
    float3 worldPos : TEXCOORD2;
};

VS_Output Main(VS_Input input)
{
    VS_Output output;

    // World matrix
    matrix worldMatrix = world[input.instanceID];

    // Compute world position
    float4 worldPos = mul(float4(input.position, 1.0f), worldMatrix);
    output.worldPos = worldPos.xyz;

    // Compute final vertex position (clip space)
    matrix wvp = mul(worldMatrix, mul(viewMatrix, projectionMatrix));
    output.position = mul(float4(input.position, 1.0f), wvp);

    // Shadow projection
    float4 lightSpacePos = mul(worldPos, lightViewMatrix);
    output.positionLight = mul(lightSpacePos, lightProjectionMatrix);

    // Transform normal and tangent (ignore translation)
    float3x3 world3x3 = (float3x3)worldMatrix;
    output.normal = mul(input.normal, world3x3);
    output.tan = mul(input.tan, world3x3);

    // Pass UVs
    output.uv = input.uv;

    return output;
}
