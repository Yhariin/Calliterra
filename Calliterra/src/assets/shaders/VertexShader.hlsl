struct VSIn
{
    float3 Pos : POSITION;
    float3 Color : COLOR;
};

struct VSOut
{
    float3 Color : COLOR;
    float4 Pos : SV_POSITION;
};

VSOut main( VSIn vertexIn)
{
    VSOut vertexOut;

    vertexOut.Pos = float4(vertexIn.Pos, 1.f);
    vertexOut.Color = vertexIn.Color;

    return vertexOut;
}