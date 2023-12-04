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
	// ������ ������������ �����ϴ� �ʼ� ��ü�� �������̽� ( �X�� ���ٽ� �䵵 ������ ���� ������� �ʴ´�.)
	ID3D11Device* m_pDevice = nullptr;						// ����̽�	
	ID3D11DeviceContext* m_pDeviceContext = nullptr;		// ��� ����̽� ���ؽ�Ʈ
	IDXGISwapChain* m_pSwapChain = nullptr;					// ����ü��
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;	// ������ Ÿ�ٺ�
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;  // ���̰� ó���� ���� �X�����ٽ� ��

	// ������ ���������ο� �����ϴ�  ��ü�� ����
	ID3D11VertexShader* m_pVertexShader = nullptr;	// ���� ���̴�.
	ID3D11PixelShader* m_pPixelShader = nullptr;	// �ȼ� ���̴�.	
	ID3D11PixelShader* m_pPixelShaderSolid = nullptr;	// �ȼ� ���̴� ����Ʈ ǥ�ÿ�.	
	ID3D11InputLayout* m_pInputLayout = nullptr;	// �Է� ���̾ƿ�.
	ID3D11Buffer* m_pVertexBuffer = nullptr;		// ���ؽ� ����.
	UINT m_VertexBufferStride = 0;					// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;					// ���ؽ� ������ ������.
	ID3D11Buffer* m_pIndexBuffer = nullptr;			// ���ؽ� ����.
	ID3D11ShaderResourceView* m_pTextureRV = nullptr;	// �ؽ�ó ���ҽ� ��.
	ID3D11ShaderResourceView* m_pNormalMap = nullptr;	// �븻�� ���ҽ� ��
	ID3D11ShaderResourceView* m_pSpecularMap = nullptr;	// ����ŧ���� ���ҽ� ��
	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// ���÷� ����.
	ID3D11BlendState* m_AlphaBlendState = nullptr;          // ���÷� ����
	int m_nIndices = 0;								// �ε��� ����.

	std::wstring m_FBXFileName;

	ID3D11Buffer* m_pConstantBuffer = nullptr;		// ��� ����.
	ID3D11Buffer* m_pCB_Lighting = nullptr;

	ConstantBuffer m_Transform;
	CB_Lighting m_Lighting;

	Vector4 m_ClearColor = Vector4(0.815f, 0.5f, 0.984f, 1.0f);
	// ���̴��� �� ������ ������

	Matrix                m_ViewTransform;				// ī�޶���ǥ�� �������� ��ȯ�� ���� ���.
	Matrix                m_ProjectionTransform;			// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.
	
	Vector3 m_SunTrans = { 0.0f, 0.0f, 0.0f };
	Vector3 m_EarthTrans = { 4.0f, 0.0f, 0.0f };
	Vector3 m_MoonTrans = { 4.0f, 0.0f, 0.0f };
	Vector3 m_CameraTrans = { 0.0f, 400.0f, -700.0f };

	float m_SunRotateSpeed[3] = { 1.0f, 1.0f, 1.0f };
	float m_EarthRotateSpeed = 5.0f;
	float m_MoonRotateSpeed = 10.0f;

	// ī�޶� ����
	float m_Near = 0.1f;
	float m_Far = 1000.f;
	float m_fovAngle = 90.0f;
	float m_fovAngleRad = 0.0f;
	float m_CameraRotate[3] = { 0.0f, 0.0f, 0.0f };
	float m_CameraScale[3] = { 1.0f, 1.0f, 1.0f };
	// ť��
	float m_Rotate[3] = { 0.0f, 0.0f, 0.0f };
	// Lighting
	XMFLOAT4 m_LightColors = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector3 m_LightDirs = { 0.0f, 0.0f, 1.0f };
	//XMFLOAT4 m_LightDirsEvaluated[2] = {};		// ���� ����Ʈ ����

	//ImGUI
	bool m_show_another_window = false;
	//bool m_show_demo_window = true;
	//float m_f;
	//int m_counter;

	virtual bool Initialize(UINT Width, UINT Height);	// ������ ������ ���� ���� �ٸ��� �����Ƿ� ���,����,���̱⸸ �Ѵ�.
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

