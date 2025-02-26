struct PS_Input
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tan : TANGENT;
    float2 uv : TEXCOORD;
};

Texture2D tex_color : register(t0);
Texture2D tex_normal : register(t1);
Texture2D tex_emission : register(t2);
Texture2D tex_mro : register(t3);
SamplerState samplerState;

cbuffer c_buffer : register(b0)
{
    float4 camDir;
    float4 lightDir;
    float4 lightDiffuse;
    float lightAmbient;
    float lightIntensity;
};

float4 Main(PS_Input input) : SV_Target
{
    // Textures
    const float3 sampleColor = tex_color.Sample(samplerState, input.uv).rgb; 
    const float3 sampleEmission = tex_emission.Sample(samplerState, input.uv).rgb; 
    const float3 sampleMRO = tex_mro.Sample(samplerState, input.uv).rgb; 
    const float3 sampleNormal = tex_normal.Sample(samplerState, input.uv).rgb;
    
    // values from textures samples
    const float AOFromSample = sampleMRO.b;
    const float SpecFromSample = 1.0f - sampleMRO.g;
    
    // Normal
    const float3 tangent = normalize(input.tan - dot(input.tan, input.normal) * input.normal);
    const float3 biTangent = cross(input.normal, tangent);
    const float3x3 normalTexSpace = float3x3(
        normalize(tangent),
        normalize(biTangent),
        normalize(input.normal)
    );
    const float3 normal = normalize(mul((2.0f * sampleNormal) - 1.0f, normalTexSpace).rgb);
    
    // Ambient
    float3 ambient = lightDiffuse.rgb * lightAmbient * sampleColor;

    // Diffuse
    float3 diffuse = max(dot(lightDir.rgb, normal), 0.0) * sampleColor;

    // Specular
    float3 halfwayDir = normalize(lightDir.rgb + camDir.rgb);
    float3 specular = lightDiffuse * lightIntensity * SpecFromSample * pow(max(dot(normal, halfwayDir), 0.0), 66.0);

    // Output
    float3 finalColor = sampleEmission + (ambient + diffuse + specular) * AOFromSample;
    return float4(finalColor, 1.0f);
}