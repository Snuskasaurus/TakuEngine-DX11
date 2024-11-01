cbuffer CBuffer
{
    float4 faceColors[6];
}

float4 main(uint VertexIndex : SV_PrimitiveID) : SV_Target
{
    return faceColors[VertexIndex / 2];
}