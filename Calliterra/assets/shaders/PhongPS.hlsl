struct VSOut
{
    float3 CameraPos : POSITION;
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
static const float attLin = 0.045f;
static const float attQuad = 0.0025f;

float4 main(VSOut pIn) : SV_TARGET
{
    const float3 vertexToLight = lightPos - pIn.CameraPos;
    const float distToLight = length(vertexToLight);
    const float3 directionToLight = vertexToLight / distToLight;

    const float att = 1.f / (attConst + attLin * distToLight + attQuad * (distToLight * distToLight));

    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(directionToLight, pIn.Normal));

    // Reflected light vector
    const float3 w = pIn.Normal * dot(vertexToLight, pIn.Normal);
    const float3 r = w * 2.0f - vertexToLight;

    // Calculate specular intensity based on angle between viewing vector
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(pIn.CameraPos))), specularPower);
    
    return float4(saturate(diffuse + ambient + specular) * MaterialColor, 1.0f);
    //return float4(pIn.Normal, 1.0f);
    
}