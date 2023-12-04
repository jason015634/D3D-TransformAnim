#include "Shared.fxh"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    // 텍스쳐
   
    
    // 노말맵핑
    float3 Normal = { 0.f, 0.f, 0.f };
    if (IsNormalMap)
    {
        float3 tangentSpace = txNormal.Sample(samLinear, input.Tex).rgb * 2.f - 1.f;
        input.Tangent = normalize(input.Tangent);
        input.BiTangent = normalize(input.BiTangent);
        input.Norm = normalize(input.Norm);
        float3x3 normalTransform = float3x3(input.Tangent, input.BiTangent, input.Norm);
        Normal = normalize(mul(tangentSpace, normalTransform));
    }
    else
    {
        Normal = normalize(input.Norm);
    }
    
    // Directional Light
    float3 DirectionLight = normalize(vLightDir.xyz);
    float NDotL = max(0, dot(-DirectionLight, Normal));
    
    // 뷰벡터 
    float3 ViewVec = normalize(EyeVector - input.WorldPos.xyz);
    //float4 finalColor = txDiffuse.Sample(samLinear, input.Tex);
    
    float4 materialColor = 1;
    if(IsDiffuseMap)
    {
        materialColor = txDiffuse.Sample(samLinear, input.Tex);
    }
    // Ambient
    float4 Ambient = m_Ambient * materialColor;
    
    // Diffuse
    float4 ColorFactor = vLightColor * materialColor;
    float4 Diffuse = m_Diffuse * ColorFactor * NDotL;
    
    // Specular(Blinn Phong)
    float HDotN = 0.0f;
    if(IsBlinnPhong)
    {
        float3 HalfVector = normalize(-DirectionLight + ViewVec);
        HDotN = max(dot(HalfVector, Normal), 0);
    }
    else
    {
        float3 Reflection = reflect(DirectionLight, Normal);
        HDotN = max(dot(Reflection, ViewVec), 0);
    }
   
    // 스페큘러
    // rgb 값은 다 같은 값이기 때문에 float3로 곱해주나 float 값으로 곱해주나 똑같음
    float3 specularMap = txSpecular.Sample(samLinear, input.Tex).rgb;
    float4 Specular = { 0.f, 0.0f, 0.f, 0.f };
    if (IsSpecularMap)
    {
        float4 specularColor = saturate(float4(m_Specular.xyz, 1));
        Specular = pow(HDotN, m_SpecularPower / 2) * specularColor * float4(specularMap, 1.0f);
    }
    else
    {
        float4 specularColor = saturate(float4(m_Specular.xyz, 1));
        Specular = pow(HDotN, m_SpecularPower / 2) * specularColor;
    }
    //float Opacity = 1.0f;
    //if (UseOpacityMap)
    //{
    //    Opacity = txOpacity.Sample(samLinear, input.Tex).a;
    //}
    //float4 finalColor = {0, };
    float4 finalColor = saturate(Ambient + Diffuse + Specular);
    //return float4(finalColor.rgb, Opacity);
    return finalColor;
}