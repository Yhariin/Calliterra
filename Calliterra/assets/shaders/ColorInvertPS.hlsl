Texture2D Tex;
SamplerState samplerState;

float4 main(float2 uv : TEXCOORD) : SV_TARGET
{
    return 1.f - Tex.Sample(samplerState, uv).rgba;

}