struct VSOut
{
    float2 uv : TEXCOORD;
    float4 pos : SV_Position;
};

VSOut main( float2 pos : POSITION )
{
    VSOut vOut;
    vOut.pos = float4(pos, 0.f, 1.f);
    vOut.uv = float2((pos.x + 1) / 2.f, -(pos.y - 1) / 2.f);

    return vOut;
}