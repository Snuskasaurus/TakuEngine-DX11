struct PS_Input
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

Texture2D texColor : register(t0);
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
    input.normal = normalize(input.normal);

    // Textures
    float tex_spec = 0.35f; // TODO: read from a texture
    float3 tex_color = texColor.Sample(samplerState, input.uv).rgb;

    // Ambient
    float3 ambient = sunDiffuse.rgb * sunAmbient * tex_color;

    // Diffuse
    float3 diffuse = max(dot(sunDir.rgb, input.normal), 0.0) * tex_color;

    // Specular
    float3 halfwayDir = normalize(sunDir.rgb + camDir.rgb);
    float3 specular = sunDiffuse * 0.3 * tex_spec * pow(max(dot(input.normal, halfwayDir), 0.0), 32.0);

    // Output
    float3 finalColor = ambient + diffuse + specular;
    return float4(finalColor, 1.0f);
}