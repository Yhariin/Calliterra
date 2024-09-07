cbuffer TransformCBuff : register(b0)
{
    matrix ViewProj;
}

struct VSOut
{
    float3 w_Pos : POSITION;
    float4 Pos : SV_Position;
};

VSOut main(float3 pos : POSITION)
{
    VSOut vOut;

    vOut.w_Pos = pos;
    vOut.Pos = mul(float4(pos, 0.f), ViewProj);

    // Make sure that the depth after w divide will be 1.0 (so that the z-buffering will work)
    vOut.Pos.z = vOut.Pos.w;

    return vOut;
}