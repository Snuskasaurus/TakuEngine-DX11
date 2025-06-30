#define PI 3.14159265
#define SHADOW_BIAS 0.0001f

cbuffer ps_buffer_sceneEachFrame : register(b0)
{
    float4 b_viewDir;
    float4 b_lightDir;
    float4 b_lightColor;
    float4 b_lightAmbientColor;
    float b_lightAmbient;
    float b_lightBrightness;
};

struct PS_Input
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tan : TANGENT;
    float2 uv : TEXCOORD0;
    float4 positionLight : TEXCOORD1;
};

Texture2D tex_color : register(t0);
Texture2D<float> tex_buffer_light: register(t1);
Texture2D tex_normal : register(t2);
Texture2D tex_emission : register(t3);
Texture2D tex_so : register(t4);

SamplerState samplerState : register(s0);
SamplerState SampleStateClamp : register(s1);
SamplerState SampleStateWrap : register(s2);

float3 ComputePhongLighting(float3 objectColor, float specularStrength, float occlusion, float3 normal)
{
    float3 N = normalize(normal);
    float3 L = normalize(b_lightDir.rgb);
    float3 V = normalize(b_viewDir.rgb);

    // Ambient: colored ambient light with occlusion and object albedo
    float3 ambient = b_lightAmbientColor.rgb * b_lightAmbient * objectColor * occlusion;

    // Diffuse
    float NdotL = saturate(dot(N, L));
    float3 diffuse = b_lightColor.rgb * b_lightBrightness * NdotL * objectColor;

    // Specular
    float3 H = normalize(L + V);
    float NdotH = saturate(dot(N, H));
    const float shininess = 64.0;
    float3 specular = b_lightColor.rgb * b_lightBrightness * specularStrength * pow(NdotH, shininess);

    return ambient + diffuse + specular;
}

float4 Main(PS_Input input) : SV_Target
{
    // Textures
    const float3 sampleColor = tex_color.Sample(samplerState, input.uv).rgb; 
    const float3 sampleEmission = tex_emission.Sample(samplerState, input.uv).rgb; 
    const float3 sampleSO = tex_so.Sample(samplerState, input.uv).rgb; 
    const float3 sampleNormal = tex_normal.Sample(samplerState, input.uv).rgb;
    
    //const float specFromSample = sampleSO.r;
    const float specFromSample = 0.0f;
    //return float4(specFromSample.xxx, 1.0f);
    const float occlusionFromSample = sampleSO.g;
    
    // Normal
    const float3 tangent = normalize(input.tan - dot(input.tan, input.normal) * input.normal);
    const float3 biTangent = cross(input.normal, tangent);
    const float3x3 normalTexSpace = float3x3(
        normalize(tangent),
        normalize(biTangent),
        normalize(input.normal)
    );
    const float3 normal = normalize(mul((2.0f * sampleNormal) - 1.0f, normalTexSpace).rgb);

    float LightMultiplier = 1.0f;
    
    float2 projectTexCoord;
    projectTexCoord.x = input.positionLight.x / input.positionLight.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.positionLight.y / input.positionLight.w / 2.0f + 0.5f;
    if((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
        //LightMultiplier = 0.8f; // Enable to debug | in shadowmap space
        float depthValue = tex_buffer_light.Sample(SampleStateClamp, projectTexCoord).r;
        float lightDepthValue = input.positionLight.z / input.positionLight.w - SHADOW_BIAS;
        if(lightDepthValue > depthValue)
        {
            LightMultiplier = 0.32f;
        }
    }
    
    // Output
    const float3 lightingColor = ComputePhongLighting(sampleColor, specFromSample, occlusionFromSample, normal);
    float3 finalColor = sampleEmission + lightingColor;
    return float4(finalColor * LightMultiplier, 1.0f);
}