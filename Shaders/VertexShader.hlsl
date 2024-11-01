struct VSOut
{
    float4 position : SV_Position;
};

cbuffer CBuffer
{
    matrix transform;
}

VSOut main(float3 position : Position)
{
    VSOut vso;
    vso.position = mul(float4(position, 1.0f), transform);
    return vso;
}