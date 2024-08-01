struct VSOut
{
    float3 worldPos : POSITION;
    float3 Normal : NORMAL;
    float4 Pos : SV_POSITION;
};


cbuffer LightCBuffer : register(b2)
{
    float3 lightPos;
};

cbuffer MaterialColorCBuffer : register(b1)
{
    float3 MaterialColor;
};

static const float3 ambient = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseColor = float3(1.f, 1.f, 1.f);
static const float diffuseIntensity = 1.0f;
static const float attConst = 1.0f;
static const float attLin = 0.045f;
static const float attQuad = 0.0075f;

float4 main(VSOut pIn) : SV_TARGET
{
    const float3 vertexToLight = lightPos - pIn.worldPos;
    const float distToLight = length(vertexToLight);
    const float3 directionToLight = vertexToLight / distToLight;

    const float att = 1.f / (attConst + attLin * distToLight + attQuad * (distToLight * distToLight));

    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(directionToLight, pIn.Normal));

    return float4(saturate(diffuse + ambient) * MaterialColor, 1.0f);
}