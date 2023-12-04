//--------------------------------------------------------------------------------------
// File: Tutorial06.fx
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License (MIT).
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txSpecular : register(t2);
Texture2D txEmissive : register(t3);
Texture2D txOpacity : register(t4);

SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

cbuffer Lighting : register(b1)
{
    float3 vLightDir;   
    float DL_pad0;      // 16
    float4 vLightColor; // 16
    float4 m_Ambient;  // 16
    float4 m_Diffuse;  // 16
    float4 m_Specular;  // 16
    float4 m_Emissive; // 16
    float3 EyeVector; 
    float pad3; // 16
    bool IsDiffuseMap; // 4
    bool IsNormalMap; // 8
    bool IsSpecularMap; // 12
    bool UseEmissiveMap; // 16
    bool UseOpacityMap; // 4
    bool IsBlinnPhong; // 8
    float m_SpecularPower; // 12
    float MaterialPad0; // 16
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
    float3 Norm : NORMAL;
    float3 Tangent : TANGENT;
};

struct PS_INPUT 
{
    float4 ProjPos : SV_POSITION;
    float4 WorldPos : POSITION;
    float2 Tex : TEXCOORD0;
    float3 Norm : TEXCOORD1;
    float3 Tangent : TEXCOORD2;
    float3 BiTangent : TEXCOORD3;
};







