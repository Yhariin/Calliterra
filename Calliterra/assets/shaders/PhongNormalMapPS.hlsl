struct VSOut
{
    float3 ViewSpacePos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
    float2 Texture : TEXCOORD;
    float4 Pos : SV_POSITION;
};

cbuffer Transforms : register(b0)
{
    matrix Model;
    matrix ModelView;
    matrix ModelViewProj;
    matrix NormalMatrix;
};

cbuffer LightCBuffer : register(b2)
{
    float3 lightPos;
};

cbuffer ObjectCBuffer : register(b1)
{
    float specularIntensity;
    float specularPower;
};

Texture2D tex : register(t0);
Texture2D nmap : register(t1);
SamplerState samplerState;

static const float3 ambient = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseColor = { 1.f, 1.f, 1.f };
static const float diffuseIntensity = 1.0f;
static const float attConst = 1.0f;
static const float attLin = 0.015f;
static const float attQuad = 0.0015f;

float4 main(VSOut pIn) : SV_TARGET
{
    
    const float3x3 TanToView = float3x3(
        normalize(pIn.Tangent),
        normalize(pIn.Bitangent),
        normalize(pIn.Normal)
    );

    const float3 normalSample = nmap.Sample(samplerState, pIn.Texture).xyz;
    pIn.Normal = normalSample * 2.f - 1.f;
    pIn.Normal.y *= -1.f;

    pIn.Normal = normalize(mul(pIn.Normal, TanToView));
    
    const float3 fragmentToLight = lightPos - pIn.ViewSpacePos;
    const float distToLight = length(fragmentToLight);
    const float3 directionToLight = normalize(fragmentToLight);

    const float att = 1.f / (attConst + attLin * distToLight + attQuad * (distToLight * distToLight));

    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(directionToLight, pIn.Normal));

    // Reflected light vector
    const float3 r = reflect(-directionToLight, pIn.Normal);

    // Calculate specular intensity based on angle between viewing vector
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(r, normalize(-pIn.ViewSpacePos))), specularPower);
    

    //return float4(float2(pIn.Texture), 0.0f, 1.0f); // Show UV colors
    //return float4(pIn.Normal, 1.0f); // Show normal colors
    
    return float4(saturate((diffuse + ambient) * tex.Sample(samplerState, pIn.Texture).rgb + specular), 1.0f);
}