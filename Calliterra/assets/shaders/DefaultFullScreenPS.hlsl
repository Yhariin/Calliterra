Texture2D Tex;
SamplerState samplerState;

float4 main(float2 uv : TEXCOORD) : SV_TARGET
{
    return Tex.Sample(samplerState, uv).rgba;
}