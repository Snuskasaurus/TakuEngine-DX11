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

float4 Main(PS_Input input) : SV_Target
{
    input.normal = normalize(input.normal);

    float3 Color = texColor.Sample(samplerState, input.uv);
    float Ambiant = worldLightAmbient;
    float Diffuse = max(dot(worldLightDir, input.normal), 0.0);
    float Specular = pow(Diffuse, 32.0) * 4.2;
    
    float3 finalColor = Color * (Ambiant + Diffuse + Specular);
    
    return float4(min(finalColor, 1.0), 1.0);
}