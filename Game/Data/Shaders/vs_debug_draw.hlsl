#define MAX_INSTANCE_COUNT 416

struct VS_Input
{
    uint index : INDEX;
    uint instanceID : SV_InstanceID;
};

struct debug_line
{
    matrix worldPosition[2];
    float3 color;
};

cbuffer vs_buffer_sceneEachFrame : register(b0)
{
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
};

cbuffer vs_buffer_debug_draw_line : register(b13)
{
    debug_line debug_lines[MAX_INSTANCE_COUNT];
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_Output Main(VS_Input input)
{
    VS_Output output;

    output.color = float4(debug_lines[input.instanceID].color, 1.0f);
    
    matrix wvp = mul(debug_lines[input.instanceID].worldPosition[input.index], mul(viewMatrix, projectionMatrix));
    
    output.position = mul(float4(0.0f, 0.0f, 0.0f, 1.0f), wvp);
    
    return output;
}