#include "Common/PointLightCBuff.hlsl"
#include "Common/ShaderOperations.hlsl"
#include "Common/LightVectorData.hlsl"
#include "Common/VSOut_PosNormTex.hlsl"

Texture2D tex : register(t0);
Texture2D specMap : register(t1);
SamplerState samplerState;

float4 main(VSOut pIn) : SV_TARGET
{
    const float4 diffuseTex = tex.Sample(samplerState, pIn.Texture);
    clip(diffuseTex.a < 0.1f ? -1 : 1);

    LightVectorData lightVectorData = CalculateLightVectorData(v_LightPos, pIn.v_Pos);

    const float attenuation = CalculateAttenuation(AttConst, AttLin, AttQuad, lightVectorData.DistToLight);

    const float3 diffuse = CalculateDiffuse(DiffuseColor, DiffuseIntensity, attenuation, lightVectorData.DirToLight, pIn.v_Normal);

    float specularPower;
    float3 specularReflectionColor;
    MapSpecular(pIn.v_Pos, pIn.v_Normal, lightVectorData.DirToLight, pIn.Texture, attenuation, DiffuseColor, DiffuseIntensity, specMap, samplerState, specularPower, specularReflectionColor);

    const float3 specular = CalculateSpecular(specularReflectionColor, 1.f, pIn.v_Normal, lightVectorData.DirToLight, lightVectorData.HalfwayDir, pIn.v_Pos, attenuation, specularPower);

    return float4(saturate((diffuse + Ambient) * diffuseTex.rgb + specular), diffuseTex.a);

}