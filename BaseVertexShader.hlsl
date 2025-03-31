struct VS_INPUT
{
    float4 position : POSITION;
    float3 color : COLOR;
    float3 color1 : COLOR1;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float3 color1 : COLOR1;
};

cbuffer object : register(b0)
{
    row_major float4x4 worldMatrix;
    float time;
};

cbuffer view : register(b1)
{
    row_major float4x4 viewMatrix;
    row_major float4x4 projMatrix;
};

cbuffer editor : register(b1)
{
    bool wireframe;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    //output.position = lerp(input.position, input.position1, (sin(m_angle) + 1.0f) / 2.0f);

    // World Space
    output.position = mul(input.position, worldMatrix);

    // View Space
    output.position = mul(output.position, viewMatrix);

    // Screen Space
    output.position = mul(output.position, projMatrix);

    output.color = input.color;
    output.color1 = input.color1;

    return output;
}