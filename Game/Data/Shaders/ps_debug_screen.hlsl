
Texture2D<float> depthBuffer; // Red contains depth.
//Texture2D<uint2> stencilBuffer; // Green contains stencil. Red is unused.

SamplerState samplerState;

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 Main(VS_OUTPUT input) : SV_Target
{
    const float1 sampleDepth = depthBuffer.Sample(samplerState, input.uv);
    
    float1 zNear = 0.1f;
    float1 zFar = 300.0f;
    
    float linearDepth = (2.0f * zNear) / (zFar + zNear - sampleDepth * (zFar - zNear));
    
    return float4(linearDepth, linearDepth, linearDepth, 1.0f);
}