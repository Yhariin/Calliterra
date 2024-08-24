#include "Common/TransformsCBuff.hlsl"

struct VSIn
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
};

struct VSOut
{
    float3 v_Pos: POSITION;
    float3 v_Normal : NORMAL;
    float4 Pos : SV_POSITION;
};

VSOut main(VSIn vIn)
{
    VSOut vOut;
    vOut.Pos = mul(float4(vIn.Position, 1.0f), ModelViewProj);
    vOut.v_Pos = (float3)mul(float4(vIn.Position, 1.0f), ModelView);
    vOut.v_Normal = normalize(mul(vIn.Normal, (float3x3) NormalMatrix));

    return vOut;
}