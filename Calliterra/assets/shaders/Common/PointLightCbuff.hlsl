cbuffer PointLightCBuff : register(b2)
{
    float3 v_LightPos;
    float3 Ambient;
    float3 DiffuseColor;
    float DiffuseIntensity;
    float AttConst;
    float AttLin;
    float AttQuad;
};