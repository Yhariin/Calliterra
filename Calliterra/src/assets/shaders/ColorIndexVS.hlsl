struct VSIn
{
    float3 Pos : POSITION;
};

struct VSOut
{
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

    return vertexOut;
}