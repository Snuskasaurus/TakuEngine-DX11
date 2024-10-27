struct VSOut
{
    float3 color : Color;
    float4 position : SV_Position;
};

cbuffer CBuffer
{
    matrix transform;
}

VSOut main(float2 pos : Position, float3 color : Color)
{
    VSOut vso;
    vso.position = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    vso.color = color;
    return vso;
}