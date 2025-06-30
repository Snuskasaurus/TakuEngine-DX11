#define PI 3.14159265
#define SHADOW_BIAS 0.0001f

cbuffer ps_buffer_sceneEachFrame : register(b0)
{
    float4 b_viewDir;
    float4 b_lightDir;
    float4 b_lightColor;
    float4 b_lightAmbientColor;
    float  b_lightAmbient;
    float  b_lightBrightness;
};

struct PS_Input
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tan : TANGENT;
    float2 uv : TEXCOORD0;
    float4 positionLight : TEXCOORD1;
    float3 worldPos : TEXCOORD2;
};

Texture2D tex_color       : register(t0);
Texture2D<float> tex_shadow : register(t1);
Texture2D tex_normal      : register(t2);
Texture2D tex_emission    : register(t3);
Texture2D tex_so          : register(t4);

SamplerState samplerState     : register(s0);
SamplerState SampleStateClamp : register(s1);
SamplerState SampleStateWrap  : register(s2);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float3 ComputePhongLighting( float3 objectColor, float specularStrength, float occlusion,float3 normal, float3 worldPos)
{
    float3 N = normalize(normal);
    float3 L = normalize(b_lightDir.xyz);
    float3 V = normalize(b_viewDir.xyz - worldPos);
    float3 H = normalize(L + V);

    // Ambient lighting
    float3 ambient = b_lightAmbientColor.rgb * b_lightAmbient * objectColor * occlusion;

    // Diffuse lighting
    float NdotL = saturate(dot(N, L));
    float3 diffuse = b_lightColor.rgb * b_lightBrightness * NdotL * objectColor;

    // Specular lighting (Blinn-Phong)
    float NdotH = saturate(dot(N, H));
    const float shininess = 64.0f;
    float3 specular = b_lightColor.rgb * b_lightBrightness * specularStrength * pow(NdotH, shininess);

    return ambient + diffuse + specular;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4 Main(PS_Input input) : SV_Target
{
    // Sample textures
    float3 albedo       = tex_color.Sample(samplerState, input.uv).rgb;
    float3 emission     = tex_emission.Sample(samplerState, input.uv).rgb;
    float3 normalMap    = tex_normal.Sample(samplerState, input.uv).rgb;
    float3 soMap        = tex_so.Sample(samplerState, input.uv).rgb;

    float specularStrength = soMap.r;
    float occlusion = 1.0f - soMap.g;

    // Construct TBN matrix
    float3 T = normalize(input.tan - dot(input.tan, input.normal) * input.normal);
    float3 B = normalize(cross(input.normal, T));
    float3 N = normalize(input.normal);
    float3x3 TBN = float3x3(T, B, N);

    // Convert normal map sample to world-space normal
    float3 normal = normalize(mul((normalMap * 2.0f - 1.0f), TBN));

    // Shadow Map Sampling
    float shadowFactor = 1.0f;

    float2 projTexCoord;
    projTexCoord.x = input.positionLight.x / input.positionLight.w * 0.5f + 0.5f;
    projTexCoord.y = -input.positionLight.y / input.positionLight.w * 0.5f + 0.5f;

    if (all(projTexCoord >= 0.0f) && all(projTexCoord <= 1.0f))
    {
        float shadowMapDepth = tex_shadow.Sample(SampleStateClamp, projTexCoord);
        float currentDepth = input.positionLight.z / input.positionLight.w - SHADOW_BIAS;

        if (currentDepth > shadowMapDepth)
            shadowFactor = 0.32f; // In shadow
    }

    // Lighting
    float3 litColor = ComputePhongLighting(albedo, specularStrength, occlusion, normal, input.worldPos);
    float3 finalColor = emission + litColor * shadowFactor;

    return float4(finalColor, 1.0f);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////