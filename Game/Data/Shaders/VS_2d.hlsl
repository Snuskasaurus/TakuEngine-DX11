struct VS_Output
{
    float4 position : SV_POSITION;
};

VS_Output Main(float4 inPos : POSITION)
{
    VS_Output output;
    output.position = float4(inPos.xyz, 01.0f);
    return output;
}