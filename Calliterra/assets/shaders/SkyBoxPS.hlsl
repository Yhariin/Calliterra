TextureCube tex : register(t0);
SamplerState samplerState : register(s0);

float4 main(float3 worldPos : POSITION) : SV_TARGET
{
    return tex.Sample(samplerState, worldPos);
}