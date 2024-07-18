struct VSOut
{
    float3 Color : COLOR;
    float4 Pos : SV_POSITION;
};

float4 main(VSOut pixelIn) : SV_TARGET
{
    float4 outColor = float4(pixelIn.Color, 1.0f);
    outColor.g *= pixelIn.Pos.x;
    return outColor;
}