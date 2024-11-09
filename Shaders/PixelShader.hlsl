struct PS_Input
{
    float2 uv : UV;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

Texture2D texColor : register(t0);
SamplerState samplerState;

cbuffer c_buffer : register(b0)
{
    float3 worldLightDir;
    float worldLightAmbient; 
};

float4 main(PS_Input input) : SV_Target
{
    input.normal = normalize(input.normal);

    float4 Color = texColor.Sample(samplerState, input.uv);
    float4 AmbiantColor = Color * worldLightAmbient;
    float3 finalColor = AmbiantColor + saturate(dot(worldLightDir, input.normal) * Color);
    
    return float4(finalColor, 1.0f);
}