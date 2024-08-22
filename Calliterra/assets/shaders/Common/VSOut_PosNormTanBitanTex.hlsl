struct VSOut
{
    float3 v_Pos: POSITION;
    float3 v_Normal : NORMAL;
    float3 v_Tangent : TANGENT;
    float3 v_Bitangent : BITANGENT;
    float2 Texture : TEXCOORD;
    float4 Pos : SV_POSITION;
};
