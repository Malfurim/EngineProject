Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
    /*
    float4 textureColor;
    
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    return textureColor;
    */
    float4 color = shaderTexture.Sample(SampleType, input.tex);
    if (color.r < 0.2f && color.g < 0.2f && color.b < 0.2f)
        color.a = 0.3f;
    
    if (color.g > 0.75f)
        color.a = 0.0f;
    //return shaderTexture.Sample(SampleType, input.tex);
    return color;
}