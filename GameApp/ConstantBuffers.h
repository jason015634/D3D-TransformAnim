#pragma once
#include <directxtk/SimpleMath.h>

using namespace DirectX::SimpleMath;
struct ConstantBuffer
{
	Matrix mWorld;
	Matrix mView;
	Matrix mProjection;
};

// 크기를 항상 16의 배수로 맞춰줘야함 
struct CB_Lighting
{
	Vector3 vLightDir = { 0,0,1 };			//12
	float pad0;					//16
	Vector4 vLightColor = { 1,1,1,1 };		//16
	Vector4 Ambient = { 0.2f,0.2f,0.2f,1.0f }; // 16
	Vector4 Diffuse = { 1.0f,1.0f,1.0f,1.0f }; // 16
	Vector4 Specular = { 1.0f,1.0f,1.0f,1.0f }; // 16
	Vector4 Emissive = { 1.0f,1.0f,1.0f,1.0f };
	Vector3 EyeVector;	//12
	float pad3r;		//16
	bool UseDiffuseMap = true; // 1
	bool boolPad0[3]; // 4
	bool UseNormalMap = true; // 5	
	bool boolPad5[3];	//8
	bool UseSpecularMap = true; // 9 
	bool boolPad1[3];	//12
	bool UseEmissiveMap = true; // 13 
	bool MT_pad3[3]; //16
	bool UseOpacityMap = true; // 1 
	bool MT_pad4[3]; // 4
	bool UseBlinnPhong = true; // 5
	bool boolPad2[3]; // 8
	float SpecularPower = 2000.0f; // 12 
	float MT_pad5; // 16
};


static_assert((sizeof(CB_Lighting) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");
