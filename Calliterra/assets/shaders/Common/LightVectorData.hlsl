struct LightVectorData
{
    float3 v_PosToLight;
    float3 DirToLight;
    float DistToLight;
};

LightVectorData CalculateLightVectorData(const float3 lightPos, const float3 fragPos)
{
    LightVectorData lv;
    lv.v_PosToLight = lightPos - fragPos;
    lv.DistToLight = length(lv.v_PosToLight);
    lv.DirToLight = normalize(lv.v_PosToLight);
    
    return lv;
}