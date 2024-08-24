#include "Common/PointLightCBuff.hlsl"
#include "Common/ShaderOperations.hlsl"
#include "Common/LightVectorData.hlsl"
#include "Common/VSOut_PosNormTex.hlsl"

cbuffer ObjectCBuffer : register(b1)
{
    float SpecularIntensity;
    float SpecularPower;
};

Texture2D tex : register(t0);
SamplerState samplerState;

float4 main(VSOut pIn) : SV_TARGET
{
    const float4 diffuseTex = tex.Sample(samplerState, pIn.Texture);
    clip(diffuseTex.a < 0.1f ? -1 : 1);

    LightVectorData lightVectorData = CalculateLightVectorData(v_LightPos, pIn.v_Pos);

    const float attentuation = CalculateAttenuation(AttConst, AttLin, AttQuad, lightVectorData.DistToLight);

    const float3 diffuse = CalculateDiffuse(DiffuseColor, DiffuseIntensity, attentuation, lightVectorData.DirToLight ,pIn.v_Normal);

    const float3 specular = CalculateSpecular(DiffuseColor, SpecularIntensity, pIn.v_Normal, lightVectorData.DirToLight, lightVectorData.HalfwayDir, pIn.v_Pos, attentuation, SpecularPower);

    return float4(saturate((diffuse + Ambient) * diffuseTex.rgb + specular), diffuseTex.a);

}