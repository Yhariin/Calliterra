#include "Common/PointLightCBuff.hlsl"
#include "Common/ShaderOperations.hlsl"
#include "Common/LightVectorData.hlsl"
#include "Common/VSOut_PosNormTex.hlsl"

cbuffer ObjectCBuffer : register(b1)
{
    float SpecularIntensity;
    float SpecularPower;
};

cbuffer SunCBuffer : register(b3)
{
    float3 SunDirection;
    float3 SunColor;
    float3 SunAmbient;
    float SunIntensity;
}

Texture2D tex : register(t0);
SamplerState samplerState;

float4 main(VSOut pIn) : SV_TARGET
{
    const float4 diffuseTex = tex.Sample(samplerState, pIn.Texture);
    clip(diffuseTex.a < 0.1f ? -1 : 1);

    LightVectorData lightVectorData = CalculateLightVectorData(v_LightPos, pIn.v_Pos);

    const float attentuation = CalculateAttenuation(AttConst, AttLin, AttQuad, lightVectorData.DistToLight);

    const float3 diffuse = CalculateDiffuse(DiffuseColor, DiffuseIntensity, attentuation, lightVectorData.DirToLight ,pIn.v_Normal);
    const float3 sunDiffuse = CalculateDiffuse(SunColor, SunIntensity, 1.f, -SunDirection, pIn.v_Normal);

    const float3 specular = CalculateSpecular(DiffuseColor, SpecularIntensity, pIn.v_Normal, lightVectorData.HalfwayDir, attentuation, SpecularPower);

    const float3 sunHalf = normalize(-SunDirection - normalize(pIn.v_Pos));
    const float3 sunSpecular = CalculateSpecular(SunColor, SpecularIntensity, pIn.v_Normal, sunHalf, 1.f, SpecularPower);

    return float4(saturate((diffuse + sunDiffuse + Ambient + SunAmbient) * diffuseTex.rgb) + specular + sunSpecular, diffuseTex.a);

}