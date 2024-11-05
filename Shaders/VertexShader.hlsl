struct VSOut
{
    float2 texCoord : TexCoord;
    float4 position : SV_Position;
};

cbuffer CBuffer
{
    matrix transform;
}

VSOut main(float3 position : Position, float2 texCoord: TexCoord)
{
    VSOut vso;
    vso.texCoord = texCoord;
    vso.position = mul(float4(position, 1.0f), transform);
    return vso;
}