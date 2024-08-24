struct LightVectorData
{
    float3 PosToLight;
    float3 DirToLight;
    float3 HalfwayDir;
    float DistToLight;
};

LightVectorData CalculateLightVectorData(const float3 v_LightPos, const float3 v_FragPos)
{
    LightVectorData lv;
    lv.PosToLight = v_LightPos - v_FragPos;
    lv.DistToLight = length(lv.PosToLight);
    lv.DirToLight = normalize(lv.PosToLight);
    lv.HalfwayDir = normalize(lv.DirToLight - normalize(v_FragPos));
    
    return lv;
}