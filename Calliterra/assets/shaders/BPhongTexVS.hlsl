#include "Common/TransformsCBuff.hlsl"
#include "Common/VSOut_PosNormTex.hlsl"

struct VSIn
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 Texture : TEXCOORD;
};

VSOut main(VSIn vIn)
{
    VSOut vOut;
    vOut.Pos = mul(float4(vIn.Position, 1.0f), ModelViewProj);
    vOut.v_Pos= (float3)mul(float4(vIn.Position, 1.0f), ModelView);
    vOut.v_Normal = normalize(mul(vIn.Normal, (float3x3) NormalMatrix));
    vOut.Texture = vIn.Texture;

    return vOut;
}