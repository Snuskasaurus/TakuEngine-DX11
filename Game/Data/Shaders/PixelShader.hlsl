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
    float4 sunDir;
    float4 sunDiffuse;
    float sunAmbient;
};

float4 Main(PS_Input input) : SV_Target
{
    input.normal = normalize(input.normal);

    float3 diffuse = texColor.Sample(samplerState, input.uv).rgb;
    
    float3 finalColor = diffuse * sunAmbient;
    finalColor += saturate(dot(sunDir, input.normal) * sunDiffuse * diffuse);
    
    //return float4(input.normal, 1.0f);
    //return float4(diffuse, 1.0f);
    //return float4(sunDir, 1.0f);
    //return float4(sunDiffuse.rgb, 1.0f);
    //return float4(sunAmbient, sunAmbient, sunAmbient, 1.0f);
    return float4(finalColor, 1.0f);
}