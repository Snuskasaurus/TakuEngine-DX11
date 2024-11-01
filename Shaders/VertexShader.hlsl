struct VSOut
{
    float3 color : Color;
    float4 position : SV_Position;
};

cbuffer CBuffer
{
    matrix transform;
}

VSOut main(float3 position : Position, float3 color : Color)
{
    VSOut vso;
    vso.position = mul(float4(position, 1.0f), transform);
    vso.color = color;
    return vso;
}