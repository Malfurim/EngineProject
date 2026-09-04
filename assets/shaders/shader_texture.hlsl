Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};
struct VertexInputType
{
    float4 position : POSITION;
    float3 diffuse : DIFFUSE;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    float2 tex2 : TEXCOORD1;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

PixelInputType VertexShaderEntry(VertexInputType input)
{
    PixelInputType output;
    
    input.position.w = 1.0f;
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input.tex;
    
    return output;
}

float4 PixelShaderEntry(PixelInputType input) : SV_TARGET
{
    return shaderTexture.Sample(SampleType, input.tex);
}