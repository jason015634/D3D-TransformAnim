#pragma once
#include "../GameApp/GameApp.h"
#include <d3d11.h>
#include <imgui.h>
#include <directxtk/SimpleMath.h>
#include <assimp/Importer.hpp>
#include "../GameApp/Mesh.h"
#include "../GameApp/Material.h"
#include "../GameApp/Model.h"
#include "../GameApp/ConstantBuffers.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;

class MyEngine :
	public GameApp
{
public:
	MyEngine(HINSTANCE hInstance);
	~MyEngine();

	vector<Model*> m_Model;
	// 렌더링 파이프라인을 구성하는 필수 객체의 인터페이스 ( 뎊스 스텐실 뷰도 있지만 아직 사용하지 않는다.)
	ID3D11Device* m_pDevice = nullptr;						// 디바이스	
	ID3D11DeviceContext* m_pDeviceContext = nullptr;		// 즉시 디바이스 컨텍스트
	IDXGISwapChain* m_pSwapChain = nullptr;					// 스왑체인
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;	// 렌더링 타겟뷰
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;  // 깊이값 처리를 위한 뎊스스텐실 뷰

	// 렌더링 파이프라인에 적용하는  객체와 정보
	ID3D11VertexShader* m_pVertexShader = nullptr;	// 정점 셰이더.
	ID3D11PixelShader* m_pPixelShader = nullptr;	// 픽셀 셰이더.	
	ID3D11PixelShader* m_pPixelShaderSolid = nullptr;	// 픽셀 셰이더 라이트 표시용.	
	ID3D11InputLayout* m_pInputLayout = nullptr;	// 입력 레이아웃.
	ID3D11Buffer* m_pVertexBuffer = nullptr;		// 버텍스 버퍼.
	UINT m_VertexBufferStride = 0;					// 버텍스 하나의 크기.
	UINT m_VertexBufferOffset = 0;					// 버텍스 버퍼의 오프셋.
	ID3D11Buffer* m_pIndexBuffer = nullptr;			// 버텍스 버퍼.
	ID3D11ShaderResourceView* m_pTextureRV = nullptr;	// 텍스처 리소스 뷰.
	ID3D11ShaderResourceView* m_pNormalMap = nullptr;	// 노말맵 리소스 뷰
	ID3D11ShaderResourceView* m_pSpecularMap = nullptr;	// 스페큘러맵 리소스 뷰
	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// 샘플러 상태.
	ID3D11BlendState* m_AlphaBlendState = nullptr;          // 샘플러 상태
	int m_nIndices = 0;								// 인덱스 개수.

	std::wstring m_FBXFileName;

	ID3D11Buffer* m_pConstantBuffer = nullptr;		// 상수 버퍼.
	ID3D11Buffer* m_pCB_Lighting = nullptr;

	ConstantBuffer m_Transform;
	CB_Lighting m_Lighting;

	Vector4 m_ClearColor = Vector4(0.815f, 0.5f, 0.984f, 1.0f);
	// 쉐이더에 에 전달할 데이터

	Matrix                m_ViewTransform;				// 카메라좌표계 공간으로 변환을 위한 행렬.
	Matrix                m_ProjectionTransform;			// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.
	
	Vector3 m_SunTrans = { 0.0f, 0.0f, 0.0f };
	Vector3 m_EarthTrans = { 4.0f, 0.0f, 0.0f };
	Vector3 m_MoonTrans = { 4.0f, 0.0f, 0.0f };
	Vector3 m_CameraTrans = { 0.0f, 400.0f, -700.0f };

	float m_SunRotateSpeed[3] = { 1.0f, 1.0f, 1.0f };
	float m_EarthRotateSpeed = 5.0f;
	float m_MoonRotateSpeed = 10.0f;

	// 카메라 관련
	float m_Near = 0.1f;
	float m_Far = 1000.f;
	float m_fovAngle = 90.0f;
	float m_fovAngleRad = 0.0f;
	float m_CameraRotate[3] = { 0.0f, 0.0f, 0.0f };
	float m_CameraScale[3] = { 1.0f, 1.0f, 1.0f };
	// 큐브
	float m_Rotate[3] = { 0.0f, 0.0f, 0.0f };
	// Lighting
	XMFLOAT4 m_LightColors = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector3 m_LightDirs = { 0.0f, 0.0f, 1.0f };
	//XMFLOAT4 m_LightDirsEvaluated[2] = {};		// 계산된 라이트 방향

	//ImGUI
	bool m_show_another_window = false;
	//bool m_show_demo_window = true;
	//float m_f;
	//int m_counter;

	virtual bool Initialize(UINT Width, UINT Height);	// 윈도우 정보는 게임 마다 다를수 있으므로 등록,생성,보이기만 한다.
	virtual void Update();
	virtual void Render();

	bool InitD3D();
	void UninitD3D();

	bool InitScene();
	void UnInitScene();

	bool InitImGUI();
	void UnInitImGUI();

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

