struct VSIn
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
};

struct VSOut
{
    float3 worldPos : POSITION;
    float3 Normal : NORMAL;
    float4 Pos : SV_POSITION;
};

cbuffer Constantbuffer 
{
    matrix Model;
    matrix ModelViewProj;
};

VSOut main(VSIn vIn)
{
    VSOut vOut;
    vOut.Pos = mul(float4(vIn.Position, 1.0f), ModelViewProj);
    vOut.worldPos = (float3)mul(float4(vIn.Position, 1.0f), Model);
    vOut.Normal = mul(vIn.Normal, (float3x3)Model);

    return vOut;
}