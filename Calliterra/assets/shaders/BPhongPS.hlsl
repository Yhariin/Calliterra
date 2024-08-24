#include "Common/PointLightCBuff.hlsl"
#include "Common/ShaderOperations.hlsl"
#include "Common/LightVectorData.hlsl"

struct VSOut
{
    float3 v_Pos : POSITION;
    float3 v_Normal : NORMAL;
    float4 Pos : SV_POSITION;
};


cbuffer ObjectCBuffer : register(b1)
{
    float3 MaterialColor;
    float SpecularIntensity;
    float SpecularPower;
};

float4 main(VSOut pIn) : SV_TARGET
{
    pIn.v_Normal = normalize(pIn.v_Normal);
    LightVectorData lightVectorData = CalculateLightVectorData(v_LightPos, pIn.v_Pos);

    const float attenuation = CalculateAttenuation(AttConst, AttLin, AttQuad, lightVectorData.DistToLight);

    const float3 diffuse = CalculateDiffuse(DiffuseColor, DiffuseIntensity, attenuation, lightVectorData.DirToLight, pIn.v_Normal);

    const float3 specular = CalculateSpecular(SpecularPower, SpecularIntensity, pIn.v_Normal, lightVectorData.DirToLight, lightVectorData.HalfwayDir, pIn.v_Pos, attenuation, SpecularPower);

    return float4(saturate(diffuse + Ambient + specular) * MaterialColor, 1.0f);
    
}