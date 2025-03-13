
Texture2D colorBuffer : register(t0);
Texture2D<float> depthBuffer : register(t1);

SamplerState samplerState;

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 Main(VS_OUTPUT input) : SV_Target
{
    const float3 sampleColor = colorBuffer.Sample(samplerState, input.uv);
    const float1 sampleDepth = depthBuffer.Sample(samplerState, input.uv);

    float1 zNear = 0.1f;
    float1 zFar = 300.0f;
    float linearDepth = (2.0f * zNear) / (zFar + zNear - sampleDepth * (zFar - zNear));

    float3 finalColor = lerp(linearDepth.x, sampleColor, float3(0.5f, 0.5f, 0.5f));
    
    return float4(finalColor , 1.0f);
}