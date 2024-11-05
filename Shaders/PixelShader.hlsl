Texture2D texColor : register(t0);

SamplerState samplerState;

float4 main(float2 tc : TexCoord) : SV_Target
{
    //return float4(tc, 0.0f, 1.0f);
    return texColor.Sample(samplerState, tc);
}