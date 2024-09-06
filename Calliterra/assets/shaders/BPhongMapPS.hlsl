#include "Common/VSOut_PosNormTanBitanTex.hlsl"
#include "Common/TransformsCBuff.hlsl"
#include "Common/PointLightCBuff.hlsl"
#include "Common/ShaderOperations.hlsl"
#include "Common/LightVectorData.hlsl"
#include "Common/MaterialCBuff.hlsl"

cbuffer SunCBuffer : register(b3)
{
    float3 SunDirection;
    float3 SunColor;
    float3 SunAmbient;
    float SunIntensity;
}

Texture2D tex : register(t0);
Texture2D specMap : register(t1);
Texture2D normMap : register(t2);
SamplerState samplerState;

float4 main(VSOut pIn) : SV_TARGET
{
    const float4 diffuseTex = tex.Sample(samplerState, pIn.Texture);
    clip(diffuseTex.a < 0.1f ? -1 : 1);

    if (UseNormalMap)
    {
        pIn.v_Normal = MapNormal(pIn.v_Tangent, pIn.v_Bitangent, pIn.v_Normal, pIn.Texture, normMap, samplerState);
    }
    
    LightVectorData lightVectorData = CalculateLightVectorData(v_LightPos, pIn.v_Pos);

    const float attenuation = CalculateAttenuation(AttConst, AttLin, AttQuad, lightVectorData.DistToLight);

    const float3 diffuse = CalculateDiffuse(DiffuseColor, DiffuseIntensity, attenuation, lightVectorData.DirToLight, pIn.v_Normal);
    const float3 sunDiffuse = CalculateDiffuse(SunColor, SunIntensity, 1.f, -SunDirection, pIn.v_Normal);

    float newSpecularPower = SpecularPower;
    float3 specularReflectionColor = DiffuseColor;
    if (UseSpecMap)
    {
        MapSpecular(pIn.v_Normal, lightVectorData.DirToLight, pIn.Texture, specMap, samplerState, newSpecularPower, specularReflectionColor);
    }
    
    const float3 specular = CalculateSpecular(specularReflectionColor, 1.f, pIn.v_Normal, lightVectorData.HalfwayDir, attenuation, newSpecularPower);
    const float3 sunHalf = normalize(-SunDirection - normalize(pIn.v_Pos));
    const float3 sunSpecular = CalculateSpecular(SunColor , 0.2f, pIn.v_Normal, sunHalf, 1.f, newSpecularPower);

    return float4(saturate(((saturate(diffuse + sunDiffuse + Ambient + SunAmbient) * diffuseTex.rgb) + specular + sunSpecular)), diffuseTex.a);
}