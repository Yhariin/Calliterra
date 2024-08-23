#include "Common/VSOut_PosNormTanBitanTex.hlsl"
#include "Common/TransformsCBuff.hlsl"
#include "Common/PointLightCBuff.hlsl"
#include "Common/ShaderOperations.hlsl"
#include "Common/LightVectorData.hlsl"

cbuffer ObjectCBuffer : register(b1)
{
    float specularIntensity;
    float specularPower;
};

Texture2D tex : register(t0);
Texture2D nmap : register(t2);
SamplerState samplerState;

float4 main(VSOut pIn) : SV_TARGET
{
    pIn.v_Normal = MapNormal(pIn.v_Tangent, pIn.v_Bitangent, pIn.v_Normal, pIn.Texture, nmap, samplerState);
    
    LightVectorData lightVectorData = CalculateLightVectorData(v_LightPos, pIn.v_Pos);

    const float attenuation = CalculateAttenuation(AttConst, AttLin, AttQuad, lightVectorData.DistToLight);

    const float3 diffuse = CalculateDiffuse(DiffuseColor, DiffuseIntensity, attenuation, lightVectorData.DirToLight, pIn.v_Normal);

    const float3 specular = CalculateSpecular(DiffuseColor, specularIntensity, pIn.v_Normal, lightVectorData.DirToLight, pIn.v_Pos, attenuation, specularPower);

    return float4(saturate((diffuse + Ambient) * tex.Sample(samplerState, pIn.Texture).rgb + specular), 1.0f);
}