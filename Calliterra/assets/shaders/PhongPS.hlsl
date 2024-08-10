struct VSOut
{
    float3 ViewSpacePos : POSITION;
    float3 Normal : NORMAL;
    float4 Pos : SV_POSITION;
};


cbuffer LightCBuffer : register(b2)
{
    float3 lightPos;
};

cbuffer ObjectCBuffer : register(b1)
{
    float3 MaterialColor;
    float specularIntensity;
    float specularPower;
};

static const float3 ambient = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseColor = { 1.f, 1.f, 1.f };
static const float diffuseIntensity = 1.0f;
static const float attConst = 1.0f;
static const float attLin = 0.015f;
static const float attQuad = 0.0015f;

float4 main(VSOut pIn) : SV_TARGET
{
    const float3 fragmentToLight = lightPos - pIn.ViewSpacePos;
    const float distToLight = length(fragmentToLight);
    const float3 directionToLight = normalize(fragmentToLight);

    const float att = 1.f / (attConst + attLin * distToLight + attQuad * (distToLight * distToLight));

    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(directionToLight, pIn.Normal));

    // Reflected light vector
    const float3 r = reflect(-directionToLight, pIn.Normal);

    // Calculate specular intensity based on angle between viewing vector
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(r, normalize(-pIn.ViewSpacePos))), specularPower);
    
    return float4(saturate(diffuse + ambient + specular) * MaterialColor, 1.0f);
    //return float4(pIn.Normal, 1.0f);
    
}