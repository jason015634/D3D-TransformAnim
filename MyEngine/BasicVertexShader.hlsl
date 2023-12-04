#include "Shared.fxh"

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.WorldPos = mul(input.Pos, World);
    output.ProjPos = mul(output.WorldPos, View);
    output.ProjPos = mul(output.ProjPos, Projection);
    output.Norm = normalize(mul(float4(input.Norm, 0), World).xyz);
    output.Tex = input.Tex;
    output.Tangent = normalize(mul(input.Tangent, (float3x3) World));
    output.BiTangent = cross(output.Norm, output.Tangent);
    
    return output;
}