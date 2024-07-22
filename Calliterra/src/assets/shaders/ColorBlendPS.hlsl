struct VSOut
{
    float3 Color : COLOR; 
    float4 Pos : SV_Position;
};

float4 main(VSOut pIn) : SV_TARGET
{
	return float4(pIn.Color, 1.0f);
}