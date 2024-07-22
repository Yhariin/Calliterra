struct VSIn
{
    float3 Position : POSITION;
    float3 Color : COLOR;
};

struct VSOut
{
    float3 Color : COLOR; 
    float4 Pos : SV_Position;
};

cbuffer CBuf
{
    matrix transform;
};

VSOut main( VSIn vIn )
{
    VSOut vOut;
    vOut.Pos = mul(float4(vIn.Position, 1.0f), transform);
    vOut.Color = vIn.Color;
	return vOut;
}