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
    float4 sunDir;
    float4 sunDiffuse;
    float sunAmbient;
};

float4 Main(PS_Input input) : SV_Target
{
    // Textures
    const float sampleSpec = 1.0f; // TODO: read from a texture
    const float3 sampleColor = tex_color.Sample(samplerState, input.uv).rgb; 
    const float3 sampleEmission = tex_emission.Sample(samplerState, input.uv).rgb; 
    const float3 sampleMRO = tex_mro.Sample(samplerState, input.uv).rgb; 
    const float AmbientOcclusion = sampleMRO.b;
    
    // Normal
    const float3 sampleNormal = tex_normal.Sample(samplerState, input.uv).rgb;
    const float3 tangent = normalize(input.tan - dot(input.tan, input.normal) * input.normal);
    const float3 biTangent = cross(input.normal, tangent);
    const float3x3 normalTexSpace = float3x3(
        normalize(tangent),
        normalize(biTangent),
        normalize(input.normal)
    );
    const float3 normal = normalize(mul((2.0f * sampleNormal) - 1.0f, normalTexSpace).rgb);
    
    // Ambient
    float3 ambient = sunDiffuse.rgb * sunAmbient * sampleColor;

    // Diffuse
    float3 diffuse = max(dot(sunDir.rgb, normal), 0.0) * sampleColor;

    // Specular
    float3 halfwayDir = normalize(sunDir.rgb + camDir.rgb);
    float3 specular = sunDiffuse * 0.3 * sampleSpec * pow(max(dot(normal, halfwayDir), 0.0), 66.0);

    // Output
    float3 finalColor = sampleEmission + (ambient + diffuse + specular) * AmbientOcclusion;
    return float4(finalColor, 1.0f);
}