float3 MapNormal(
    const in float3 tan, 
    const in float3 bitan, 
    const in float3 normal, 
    const in float2 textureCoord, 
    uniform Texture2D normalMap,
    uniform SamplerState samplerState)
{
    // Build a transform (rotation) into the same space as tan/bitan/normal
    const float3x3 tanToTarget = float3x3(tan, bitan, normal);

    // Sample and unpack the normal from texture into tangent space
    const float3 normalSample = normalMap.Sample(samplerState, textureCoord).xyz;
    float3 tanNormal = normalSample * 2.f - 1.f;

    // Bring normal from tangent space to target space
    return normalize(mul(tanNormal, tanToTarget));
}

void MapSpecular(
    const in float3 v_Pos,
    const in float3 v_Normal, 
    const in float3 v_DirToLight, 
    const in float2 textureCoord,
    const in float3 attenuation,
    uniform float3 diffuseColor,
    uniform float3 diffuseIntensity,
    uniform Texture2D specularMap, 
    uniform SamplerState samplerState,
    out float specularPower,
    out float3 specularReflectionColor)
{
    const float3 r = reflect(-v_DirToLight, v_Normal);

    // Calculate specular intensity based on angle between viewing vector
    const float4 specularSample = specularMap.Sample(samplerState, textureCoord);
    specularReflectionColor = specularSample.rgb;
    specularPower = pow(2.0f, specularSample.a * 13.0f);

    //return attenuation * (diffuseColor * diffuseIntensity) * pow(max(0.0f, dot(r, normalize(-v_Pos))), specularPower);
}

float CalculateAttenuation(uniform float attConst, uniform float attLin, uniform float attQuad, const float distFragToL)
{
    return 1.f / ( attConst + (attLin * distFragToL) + (attQuad * distFragToL * distFragToL) );
}

float3 CalculateDiffuse(
    uniform float3 diffuseColor,
    uniform float diffuseIntensity,
    const in float attenuation,
    const in float3 v_DirToLight,
    const in float3 v_Normal)
{
    return diffuseColor * diffuseIntensity * attenuation * max(0.f, dot(v_DirToLight, v_Normal));
}

float3 CalculateSpecular(
    const in float3 specularColor,
    uniform float specularIntensity,
    const in float3 v_Normal,
    const in float3 v_DirToLight,
    const in float3 v_Pos,
    const in float attenuation,
    const in float specularPower)
{
    // Reflected light vector
    const float3 r = reflect(-v_DirToLight, v_Normal);

    // Calculate specular intensity based on angle between viewing vector
    return attenuation * (specularColor * specularIntensity) * pow(max(0.f, dot(r, normalize(-v_Pos))), specularPower);
}