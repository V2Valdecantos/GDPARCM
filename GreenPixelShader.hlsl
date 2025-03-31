struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};


float4 psmain(PS_INPUT input) : SV_TARGET
{
    return float4(float3(0.0f, 1.0f, 0.0f), 1.0f);
}