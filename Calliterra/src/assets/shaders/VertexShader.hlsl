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

cbuffer CBuf
{
    matrix transform;
};

VSOut main( VSIn vertexIn)
{
    VSOut vertexOut;

    vertexOut.Pos = mul(float4(vertexIn.Pos, 1.0f), transform);
    vertexOut.Color = vertexIn.Color;

    return vertexOut;
}