#include "Common/TransformsCBuff.hlsl"
#include "Common/VSOut_PosNormTanBitanTex.hlsl"

struct VSIn
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
    float2 Texture : TEXCOORD;
};


VSOut main(VSIn vIn)
{
    VSOut vOut;
    vOut.Pos = mul(float4(vIn.Position, 1.0f), ModelViewProj);
    vOut.v_Pos = (float3)mul(float4(vIn.Position, 1.0f), ModelView);
    vOut.v_Normal = normalize(mul(vIn.Normal, (float3x3) NormalMatrix));

    vOut.v_Tangent = mul(vIn.Tangent, (float3x3) ModelView);
    vOut.v_Bitangent = mul(vIn.Bitangent, (float3x3) ModelView);

    vOut.v_Tangent = normalize(vOut.v_Tangent - dot(vOut.v_Tangent, vOut.v_Normal) * vOut.v_Normal);
    vOut.v_Bitangent = cross(vOut.v_Normal, vOut.v_Tangent);
    
    vOut.Texture = vIn.Texture;

    return vOut;
}