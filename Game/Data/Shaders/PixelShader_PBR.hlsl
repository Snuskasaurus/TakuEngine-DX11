#define PI 3.14159265

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
    float4 b_viewDir;
    float4 b_lightDir;
    float4 b_lightColor;
    float b_lightAmbient;
    float b_lightBrightness;
};

// Represents the number of micro-facets that reflect light back at the viewer
float ggxDistribution(float _roughness, float _nDotH) 
{
    float alpha2 = pow(_roughness, 4); // pow4 instead of pow2 because it looks better
    float d = _nDotH * _nDotH * (alpha2 - 1) + 1;
    return alpha2 / (PI * d * d);
}

// approximates micro-facets self shadowing
float geomSmith(float _roughness, float _dot)
{
    float k = (_roughness + 1.0f) * (_roughness + 1.0f) / 8.0f;
    float d = _dot * (1 - k) + k;
    return _dot / d;
}

// Simulate the fresnel reflection
float schlickFresnel(float _metallic, float _vDotH)
{
    return _metallic + (1.0f - _metallic) * pow(clamp(1.0f - _vDotH, 0.0f, 1.0f), 5.0f);
}

float3 ComputePBRLighting(float3 _objectColor, float _roughness, float _metallic, float3 _normal)
{
    const float lightIntensity = b_lightColor * b_lightBrightness;

    const float3 halfV = normalize(b_lightDir.rgb + b_viewDir.rgb);
    const float nDotL = max(dot(_normal, b_lightDir.rgb), 0.0);
    const float nDotH = normalize(dot(_normal, halfV));
    const float nDotV = normalize(dot(_normal, b_viewDir));
    const float vDotH = normalize(dot(halfV, b_viewDir));

    const float fresnelFactor = schlickFresnel(_metallic, vDotH);
    const float kS = fresnelFactor;
    const float kD = 1.0f - fresnelFactor;
    
    // Specular
    const float specularBRDF_n = ggxDistribution(_roughness, nDotH) * fresnelFactor * geomSmith(_roughness, nDotL) * geomSmith(_roughness, nDotV);
    const float specularBRDF_d = 4.0f * nDotV * nDotL + 0.0001f;
    const float3 specularBRDF = specularBRDF_n / specularBRDF_d;
    
    // Diffuse
    float3 fLambert = _objectColor * _metallic;
    const float3 diffuseBRDF = kD * fLambert / PI;
    
    return (diffuseBRDF + specularBRDF) * lightIntensity * nDotL;
}

float3 ComputePhongLighting(float3 _objectColor, float _spec, float3 _normal, float _occlusion)
{
    // Ambient
    const float3 ambient = b_lightColor.rgb * b_lightAmbient * _objectColor * _occlusion;
    
    // Diffuse
    const float3 diffuse = max(dot(b_lightDir.rgb, _normal), 0.0) * _objectColor;

    // Specular
    const float3 halfwayDir = normalize(b_lightDir.rgb + b_viewDir.rgb);
    const float3 specular = b_lightColor * b_lightBrightness * _spec * pow(max(dot(_normal, halfwayDir), 0.0), 66.0);

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
    
    // Normal
    const float3 tangent = normalize(input.tan - dot(input.tan, input.normal) * input.normal);
    const float3 biTangent = cross(input.normal, tangent);
    const float3x3 normalTexSpace = float3x3(
        normalize(tangent),
        normalize(biTangent),
        normalize(input.normal)
    );
    const float3 normal = normalize(mul((2.0f * sampleNormal) - 1.0f, normalTexSpace).rgb);
    
    // Output
    //const float3 lightingColor = ComputePhongLighting(sampleColor, specFromSample, normal, occlusionFromSample);
    const float3 lightingColor = ComputePBRLighting(sampleColor, sampleMRO.g, sampleMRO.r, normal);
    float3 finalColor = sampleEmission + lightingColor;
    return float4(finalColor, 1.0f);
}