#include "Common/PointLightCBuff.hlsl"
#include "Common/ShaderOperations.hlsl"
#include "Common/LightVectorData.hlsl"

struct VSOut
{
    float3 v_Pos: POSITION;
    float3 v_Normal : NORMAL;
    float2 Texture : TEXCOORD;
    float4 Pos : SV_POSITION;
};

Texture2D tex : register(t0);
Texture2D specMap : register(t1);
SamplerState samplerState;

float4 main(VSOut pIn) : SV_TARGET
{
    LightVectorData lightVectorData = CalculateLightVectorData(v_LightPos, pIn.v_Pos);

    const float attenuation = CalculateAttenuation(AttConst, AttLin, AttQuad, lightVectorData.DistToLight);

    const float3 diffuse = CalculateDiffuse(DiffuseColor, DiffuseIntensity, attenuation, lightVectorData.DirToLight, pIn.v_Normal);

    float3 specularReflectionColor;
    const float3 specular = MapSpecular(pIn.v_Pos, pIn.v_Normal, lightVectorData.DirToLight, pIn.Texture, attenuation, DiffuseColor, DiffuseIntensity, specMap, samplerState, specularReflectionColor);

    return float4(saturate((diffuse + Ambient) * tex.Sample(samplerState, pIn.Texture).rgb + specular * specularReflectionColor), 1.0f);

}