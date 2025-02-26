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
    float4 b_camDir;
    float4 b_lightDir;
    float4 b_lightColor;
    float b_lightAmbientIntensity;
    float b_lightColorIntensity;
};

float3 ComputePhongLighting(float3 _objectColor, float _spec, float3 _normalmap, float3 _normal, float3 _tangent, float _occlusion)
{
    // Normal
    const float3 tangent = normalize(_tangent - dot(_tangent, _normal) * _normal);
    const float3 biTangent = cross(_normal, tangent);
    const float3x3 normalTexSpace = float3x3(
        normalize(tangent),
        normalize(biTangent),
        normalize(_normal)
    );
    const float3 normal = normalize(mul((2.0f * _normalmap) - 1.0f, normalTexSpace).rgb);
    
    // Ambient
    float3 ambient = b_lightColor.rgb * b_lightAmbientIntensity * _objectColor * _occlusion;

    // Diffuse
    float3 diffuse = max(dot(b_lightDir.rgb, normal), 0.0) * _objectColor;

    // Specular
    float3 halfwayDir = normalize(b_lightDir.rgb + b_camDir.rgb);
    float3 specular = b_lightColor * b_lightColorIntensity * _spec * pow(max(dot(normal, halfwayDir), 0.0), 66.0);

    // Output
    return ambient + diffuse + specular;
}

float4 Main(PS_Input input) : SV_Target
{
    // Textures
    const float3 sampleColor = tex_color.Sample(samplerState, input.uv).rgb; 
    const float3 sampleEmission = tex_emission.Sample(samplerState, input.uv).rgb; 
    const float3 sampleMRO = tex_mro.Sample(samplerState, input.uv).rgb; 
    const float3 sampleNormal = tex_normal.Sample(samplerState, input.uv).rgb;
    
    const float specFromSample = 1.0f - sampleMRO.g;
    const float occlusionFromSample = sampleMRO.b;
    
    // Output
    const float3 lightingColor = ComputePhongLighting(sampleColor, specFromSample, sampleNormal, input.normal, input.tan, occlusionFromSample);
    float3 finalColor = sampleEmission + lightingColor;
    return float4(finalColor, 1.0f);
}