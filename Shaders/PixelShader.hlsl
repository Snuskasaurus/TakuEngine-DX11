float4 main(float3 color : Color) : SV_Target
{
    return float4(color.x, color.y, color.z, 1.0f);
}