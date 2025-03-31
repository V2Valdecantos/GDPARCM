struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
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

cbuffer editor : register(b2)
{
    bool wireframe;
}

Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
    if (wireframe)
        return float4(0.0, 0.0, 0.0, 1.0);
    return Texture.Sample(TextureSampler, input.texcoord * 0.5);
}