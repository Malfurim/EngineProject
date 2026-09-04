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
    float3 diffuse : DIFFUSE;
    float2 tex : TEXCOORD0;
};

PixelInputType VertexShaderEntry(VertexInputType input)
{
    PixelInputType output;
    
    input.position.w = 1.0f;
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.diffuse = input.diffuse;
    output.tex = input.tex;
    
    return output;
}

float4 PixelShaderEntry(PixelInputType input) : SV_TARGET
{
    /*
    float4 color;
    
    color = shaderTexture.Sample(SampleType, input.tex);
    
    if (color.r == 0.0f)
        color.a = 0.0f;
    else
    {
        color.r = color.r * input.diffuse.r;
        color.g = color.g * input.diffuse.g;
        color.b = color.b * input.diffuse.b;
        color.a = 1.0f;
    }
    
    return color;
    */
    
    float4 textureColor = shaderTexture.Sample(SampleType, input.tex);
    float4 finalColor;
    
    finalColor.rgb = input.diffuse.rgb * textureColor.rgb;
    finalColor.a = textureColor.r;
    
    return finalColor;
}
