struct VSIn
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 Texture : TEXCOORD;
};

struct VSOut
{
    float3 ViewSpacePos : POSITION;
    float3 Normal : NORMAL;
    float2 Texture : TEXCOORD;
    float4 Pos : SV_POSITION;
};

cbuffer Transforms 
{
    matrix Model;
    matrix ModelView;
    matrix ModelViewProj;
    matrix NormalMatrix;
};

VSOut main(VSIn vIn)
{
    VSOut vOut;
    vOut.Pos = mul(float4(vIn.Position, 1.0f), ModelViewProj);
    vOut.ViewSpacePos = (float3)mul(float4(vIn.Position, 1.0f), ModelView);
    vOut.Normal = normalize(mul(vIn.Normal, (float3x3) NormalMatrix));
    vOut.Texture = vIn.Texture;

    return vOut;
}