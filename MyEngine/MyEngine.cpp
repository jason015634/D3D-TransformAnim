#include "../GameApp/pch.h"
#include "MyEngine.h"
#include "../GameApp/Helper.h"
#include <d3dcompiler.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include "InputManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../GameApp/Model.h"
#include "../GameApp/ResourceLoader.h"
#include "../GameApp/ConstantBuffers.h"
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")


MyEngine::MyEngine(HINSTANCE hInstance)
	:GameApp(hInstance)
{

}

MyEngine::~MyEngine()
{
	UnInitScene();
	UninitD3D();
}

bool MyEngine::Initialize(UINT Width, UINT Height)
{
	__super::Initialize(Width, Height);
	InputManager::Init(m_hWnd);
	if (!InitD3D())
		return false;
	if (!InitScene())
		return false;
	if (!InitImGUI())
		return false;

	return true;
}

void MyEngine::Update()
{
	__super::Update();
	static float angle = 0.0f;
	float totalTime = 0.01f;// TimeSystem::GetInst()->GetDeltaTime();

	angle += 0.00002f;
	// 첫번째 큐브
	//XMMATRIX mSunOrbitX = XMMatrixRotationX(angle * m_Rotate[0]);
	//XMMATRIX mSunOrbitY = XMMatrixRotationY(angle * m_Rotate[1]);
	//XMMATRIX mSunOrbitZ = XMMatrixRotationZ(angle * m_Rotate[2]);
	//XMMATRIX mSunTrans = XMMatrixTranslation(m_SunTrans.x, m_SunTrans.y, m_SunTrans.z);
	//m_WorldSun = mSunOrbitX * mSunOrbitY * mSunOrbitZ * mSunTrans;

	static float moveSpeed = 0.01f;
	{
		//if (InputManager::IsKeyDown(Keyboard::Keys::W) == true)
		//{
		//	m_CameraTrans.z += moveSpeed;
		//}
		//if (InputManager::IsKeyDown(Keyboard::Keys::S) == true)
		//{
		//	m_CameraTrans.z -= moveSpeed;
		//}
		//if (InputManager::IsKeyDown(Keyboard::Keys::A) == true)
		//{
		//	m_CameraTrans.x -= moveSpeed;
		//}
		//if (InputManager::IsKeyDown(Keyboard::Keys::D) == true)
		//{
		//	m_CameraTrans.x += moveSpeed;
		//}
		//if (InputManager::IsKeyDown(Keyboard::Keys::Q) == true)
		//{
		//	m_CameraTrans.y += moveSpeed;
		//}
		//if (InputManager::IsKeyDown(Keyboard::Keys::E) == true)
		//{
		//	m_CameraTrans.y -= moveSpeed;
		//}
	}

	Mouse::State state = InputManager::MouseState();
	static float rotationSpeed = 0.001f;
	if (state.middleButton == true)
	{
		m_CameraTrans.x -= state.x * moveSpeed;
		m_CameraTrans.y += state.y * moveSpeed;
	}

	//if (state.leftButton == true)
	//{
	//	m_CameraRotate[1] -= state.x * rotationSpeed;
	//	m_CameraRotate[0] -= state.y * rotationSpeed;
	//}

	//if (state.scrollWheelValue > 0)
	//{
	//	m_fovAngle += (state.scrollWheelValue/ 1000.f);
	//}
	//else if (state.scrollWheelValue < 0)
	//{
	//	m_fovAngle -= (state.scrollWheelValue / 1000.f);
	//}
	
	//두번째 큐브
	//XMMATRIX mEarthSpin = XMMatrixRotationY(angle * m_EarthRotateSpeed);
	//XMMATRIX mEarthOrbit = m_WorldSun;
	//XMMATRIX mEarthTrans = XMMatrixTranslation(m_EarthTrans.x, m_EarthTrans.y, m_EarthTrans.z);
	//XMMATRIX mEarthScale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	//m_WorldEarth = mEarthScale * mEarthSpin * mEarthTrans * mEarthOrbit;

	//// 세번째 큐브
	//XMMATRIX mMoonSpin = XMMatrixRotationY(angle * m_MoonRotateSpeed);
	//XMMATRIX mMoonOrbit = m_WorldEarth;
	//XMMATRIX mMoonTrans = XMMatrixTranslation(m_MoonTrans.x, m_MoonTrans.y, m_MoonTrans.z);
	//XMMATRIX mMoonScale = XMMatrixScaling(0.5, 0.5f, 0.5f);
	//m_WorldMoon = mMoonScale * mMoonSpin * mMoonTrans * mMoonOrbit;

	// 카메라
	// 월드 공간에서의 카메라 좌표
	XMVECTOR Eye = XMVectorSet(m_CameraTrans.x, m_CameraTrans.y, m_CameraTrans.z, 0.0f);
	// 카메라의 시점에서 바라보는 지점의 좌표
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	// 카메라 상단의 방향을 나타내는 벡터
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_ViewTransform = XMMatrixLookToLH(Eye, At, Up);
	m_ViewTransform *= XMMatrixRotationRollPitchYaw(m_CameraRotate[0], m_CameraRotate[1], m_CameraRotate[2]);
	//m_Lighting.EyeVector = Eye;
	//m_ViewTransform *= XMMatrixScalingFromVector(m_CameraScale[0], m_CameraScale[1], m_CameraScale[2]);

	m_Lighting.EyeVector = m_CameraTrans;
	// 투영 변환 
	//m_fovAngleRad = m_fovAngle * (XM_PI / 180.0f);
	m_fovAngleRad = XMConvertToRadians(m_fovAngle);
	if (m_Far - m_Near < 0.1f)
	{
		m_Far = m_Near + 0.1f;
	} 

	// Initialize the projection matrix
	m_ProjectionTransform = XMMatrixPerspectiveFovLH(m_fovAngleRad, m_ClientWidth / (FLOAT)m_ClientHeight, m_Near, m_Far);


	// Rotate the second light around the origin
	XMVECTOR vLightDir = XMLoadFloat3(&m_LightDirs);
	vLightDir = XMVector3Transform(vLightDir, Matrix::Identity);
	//XMStoreFloat4(&m_LightDirsEvaluated, vLightDir);

	for (auto obj : m_Model)
	{
		obj->Update(totalTime);
	}
}

void MyEngine::Render()
{

	// 화면 칠하기.
	float color[4] = { m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0); // 뎁스버퍼 1.0f로 초기화.

	// Draw계열 함수를 호출하기전에 렌더링 파이프라인에 필수 스테이지 설정을 해야한다.	
	// 정점을 이어서 그릴 방식 설정. // 정점이 삼각형 목록으로 처리됨
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 
	// 정점 버퍼를 입력 어셈블러에 바인딩하여 파이프라인이 렌더링을 위해 정점 데이터에 액세스 할 수 있도록 한다
	//m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &m_VertexBufferOffset);
	// 정점 셰이더가 정점 데이터를 해석하는 방법을 정의
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	// 인덱스 버퍼
	//m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	// 렌더링 중에 사용할 정점 셰이더를 설정
	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	// 정점 셰이더에 사용 될 상수 버퍼 설정
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pCB_Lighting);
	// 렌더링 중에 사용될 픽셀(조각) 셰이더를 설정
	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pCB_Lighting);
	/*m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	m_pDeviceContext->PSSetShaderResources(1, 1, &m_pNormalMap);
	m_pDeviceContext->PSSetShaderResources(2, 1, &m_pSpecularMap);*/
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	

	// 첫번째 큐브 정보 업데이트
	//m_Transform.mWorld = XMMatrixTranspose(m_WorldSun);
	m_Transform.mView = XMMatrixTranspose(m_ViewTransform);
	m_Transform.mProjection = XMMatrixTranspose(m_ProjectionTransform);
	// 메인 메모리에 있는 데이터를 셰이더가 쓸 수 있는 공간(비디오 메모리)으로 복사해서 옮겨주는 함수 
	m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_Transform, 0, 0);

	m_LightDirs.Normalize();
	m_Lighting.vLightDir = m_LightDirs;
	m_Lighting.vLightColor = m_LightColors;
	//cbSun.vOutputColor = XMFLOAT4(1.0, 0, 0, 0);
	m_pDeviceContext->UpdateSubresource(m_pCB_Lighting, 0, nullptr, &m_Lighting, 0, 0);

	// Render a triangle by using Index Buffer
	//m_pDeviceContext->DrawIndexed(m_nIndices, 0, 0);

	{
		//// 두번째 큐브 정보 업데이트
		//ConstantBuffer cbEarth;
		//cbEarth.mWorld = XMMatrixTranspose(m_WorldEarth);
		//cbEarth.mView = XMMatrixTranspose(m_ViewTransform);
		//cbEarth.mProjection = XMMatrixTranspose(m_ProjectionTransform);
		//m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cbEarth, 0, 0);
		//m_pDeviceContext->DrawIndexed(m_nIndices, 0, 0);

		//// 세번째 큐브 정보 업데이트
		//ConstantBuffer cbMoon;
		//cbMoon.mWorld = XMMatrixTranspose(m_WorldMoon);
		//cbMoon.mView = XMMatrixTranspose(m_ViewTransform);
		//cbMoon.mProjection = XMMatrixTranspose(m_ProjectionTransform);
		//m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cbMoon, 0, 0);
		//m_pDeviceContext->DrawIndexed(m_nIndices, 0, 0);

		// Render each light	
		//for (int m = 0; m < 2; m++)
		//{
		//	XMMATRIX mLight = XMMatrixTranslationFromVector( XMLoadFloat4(&m_LightDirsEvaluated));
		//	XMMATRIX mLightScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
		//	mLight = mLightScale * mLight;

		//	// Update the world variable to reflect the current light
		//	cbSun.mWorld = XMMatrixTranspose(mLight);
		//	/*cbEarth.mWorld = XMMatrixTranspose(mLight);
		//	cbMoon.mWorld = XMMatrixTranspose(mLight);*/
		//	cbSun.vOutputColor = m_LightColors;
		//	//cbEarth.vOutputColor = m_LightColors[m];
		//	//cbMoon.vOutputColor = m_LightColors[m];
		//	m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cbSun, 0, 0);
		//	//m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cbEarth, 0, 0);
		//	//m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cbMoon, 0, 0);

		//	m_pDeviceContext->PSSetShader(m_pPixelShaderSolid, nullptr, 0);
		//	m_pDeviceContext->DrawIndexed(m_nIndices, 0, 0);
		//}
	}
	for (auto obj : m_Model)
	{
		obj->Render(m_pDeviceContext, m_AlphaBlendState, m_pCB_Lighting, m_pConstantBuffer, &m_Transform);
	}
	// ImGUI 
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
		ImGui::SetWindowSize(ImVec2(400.f, 300.f));
		ImGui::ColorEdit3("clear color", (float*)&m_ClearColor); // Edit 3 floats representing a color	
		ImGui::SliderFloat3("SunTranslate", (float*)&m_SunTrans, -10.0f, 10.0f);
		ImGui::DragFloat3("SunRotation", (float*)&m_Rotate, 0.01f, -10.0f, 10.0f);
		ImGui::SliderFloat3("EarthTranslate", (float*)&m_EarthTrans, -10.0f, 10.0f);
		ImGui::SliderFloat3("MoonTranslate", (float*)&m_MoonTrans, -10.0f, 10.0f);
		ImGui::SliderFloat3("CameraTranslate", (float*)&m_CameraTrans, 500.f, 500.f);
		ImGui::SliderFloat3("CameraRotate", (float*)&m_CameraRotate, -DirectX::XM_2PI, DirectX::XM_2PI);
		ImGui::SliderFloat("FovAngle", (float*)&m_fovAngle, 1.0f, 180.f);

		ImGui::SliderFloat("Near", (float*)&m_Near, 0.1f, 100.f);
		ImGui::SliderFloat("Far", (float*)&m_Far, 0.1f, 1000.f);
		ImGui::End();
	}

	{
		ImGui::Begin("Another Window", &m_show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::SetWindowSize(ImVec2(400.f, 300.f));
		ImGui::ColorEdit3("LightColor", (float*)&m_LightColors);
		ImGui::DragFloat3("LightDir", (float*)&m_LightDirs, 0.01f, -1.0f, 1.0f);
		ImGui::Checkbox("UseBlinnPhong", &m_Lighting.UseBlinnPhong);
		ImGui::Checkbox("UseNormalMap", &m_Lighting.UseNormalMap);
		ImGui::Checkbox("UseSpecularMap", &m_Lighting.UseSpecularMap);
		ImGui::ColorEdit4("Diffuse", (float*)&m_Lighting.Diffuse);
		ImGui::ColorEdit4("Ambient", (float*)&m_Lighting.Ambient);
		ImGui::ColorEdit4("Specular", (float*)&m_Lighting.Specular);
		ImGui::DragFloat("Specular", (float*)&m_Lighting.SpecularPower, 2.5f, 1.0f, 2000.0f);
		ImGui::End();
	}
	ImGui::Render();
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	// 스왑체인 교체.
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	m_pSwapChain->Present(0, 0);
}

bool MyEngine::InitD3D()
{
	// 결과값.
	HRESULT hr = 0;

	// 스왑체인 속성 설정 구조체 생성.
	// DX Graphic Interface
	DXGI_SWAP_CHAIN_DESC swapDesc = {};

	swapDesc.BufferCount = 1;	// 스왑 체인이 사용할 버퍼 개수, 더블 버퍼링에서는 2를 지정한다.
	//swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	//기본값 0, 백버퍼를 스왑 할 때 처리효과를 지정
													// 최고의 성능을 제공하고 실시간 3D 렌더링에 가장 일반적으로 사용됨

	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 후면 버퍼에 렌더링 할 것이므로 DXGI_USAGE_RENDER_TARGET_OUTPUT
	swapDesc.OutputWindow = m_hWnd;	// 렌더링 결과가 표시될 창의 핸들 
	swapDesc.Windowed = true;		// 창 모드 여부 설정.
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// 백버퍼(텍스처)의 가로/세로 크기 설정.
	swapDesc.BufferDesc.Width = m_ClientWidth;
	swapDesc.BufferDesc.Height = m_ClientHeight;
	// 화면 주사율 설정.
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	// 샘플링 관련 설정.
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// 1. 장치 생성.   2.스왑체인 생성. 3.장치 컨텍스트 생성.
	HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
		D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext));

	// 4. 렌더타겟뷰 생성.  (백버퍼를 이용하는 렌더타겟뷰)	
	ID3D11Texture2D* pBackBufferTexture = nullptr;
	HR_T(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture));
	HR_T(m_pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &m_pRenderTargetView));  // 텍스처는 내부 참조 증가
	SAFE_RELEASE(pBackBufferTexture);	//외부 참조 카운트를 감소시킨다.

	// 렌더 타겟을 최종 출력 파이프라인에 바인딩합니다.
	// FlipMode가 아닐때는 최초 한번만 설정하면 된다.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

	// 5. 뷰포트 설정.	
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)m_ClientWidth;
	viewport.Height = (float)m_ClientHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_pDeviceContext->RSSetViewports(1, &viewport);

	//6. 뎊스&스텐실 뷰 생성
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = m_ClientWidth;
	descDepth.Height = m_ClientHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ID3D11Texture2D* textureDepthStencil = nullptr;
	HR_T(m_pDevice->CreateTexture2D(&descDepth, nullptr, &textureDepthStencil));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	// 깊이 버퍼는 렌더 대상의 픽셀이 카메라에서 얼마나 멀리 있는지를 저장하는 데 사용
	HR_T(m_pDevice->CreateDepthStencilView(textureDepthStencil, &descDSV, &m_pDepthStencilView));
	SAFE_RELEASE(textureDepthStencil);

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	//7. 알파블렌딩을 위한 블렌드 상태 생성
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HR_T(m_pDevice->CreateBlendState(&blendDesc, &m_AlphaBlendState));
	return true;
}

void MyEngine::UninitD3D()
{
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDevice);
}

bool MyEngine::InitScene()
{
	HRESULT hr = 0;
	ID3D10Blob* errorMessage = nullptr; // 컴파일 에러 메세지가 저장될 버퍼

	{
		//1. Render() 에서 파이프라인에 바인딩할 버텍스 버퍼및 버퍼 정보 준비
	// 아직은 VertexShader의 World, View, Projection 변환을 사용하지 않으므로 
	// 직접 Normalized Device Coordinate(좌표계)의 위치로 설정한다.
	//      /---------------------(1,1,1)   z값은 깊이값
	//     /                      / |   
	// (-1,1,0)----------------(1,1,0)        
	//   |         v1           |   |
	//   |        /   `         |   |       중앙이 (0,0,0)  
	//   |       /  +   `       |   |
	//   |     /         `      |   |
	//	 |   v0-----------v2    |  /
	// (-1,-1,0)-------------(1,-1,0)

		/*Vertex vertices[] =
		{
			Vertex(Vector3(-1.0f, 1.0f, -1.0f),		Vector4(1.0f, 0.41f, 0.71f, 1.0f)),
			Vertex(Vector3(-1.0f, 1.0f, 1.0f),		Vector4(0.53f, 0.81f, 0.92f, 1.0f)),
			Vertex(Vector3(1.0f, 1.0f, -1.0f),		Vector4(1.0f, 0.45f, 0.0f, 1.0f)),
			Vertex(Vector3(1.0f, 1.0f, 1.0f),		Vector4(0.71f, 0.49f, 0.86f, 1.0f)),
			Vertex(Vector3(-1.0f, -1.0f, -1.0f),	Vector4(0.6f, 0.98f, 0.6f, 1.0f)),
			Vertex(Vector3(-1.0f, -1.0f, 1.0f),		Vector4(1.0f, 0.84f, 0.0f, 1.0f)),
			Vertex(Vector3(1.0f, -1.0f, -1.0f),		Vector4(0.22f, 0.69f, 0.87f, 1.0f)),
			Vertex(Vector3(1.0f, -1.0f, 1.0f),		Vector4(1.0f, 0.94f, 0.0f, 1.0f))
		};*/

		//Vertex vertices[] =
		//{
		//	{ Vector3(-1.0f, 1.0f, -1.0f),	Vector3(0.0f, 1.0f, 0.0f) },// Normal Y +	 
		//	{ Vector3(1.0f, 1.0f, -1.0f),	Vector3(0.0f, 1.0f, 0.0f) },
		//	{ Vector3(1.0f, 1.0f, 1.0f),	Vector3(0.0f, 1.0f, 0.0f) },
		//	{ Vector3(-1.0f, 1.0f, 1.0f),	Vector3(0.0f, 1.0f, 0.0f) },

		//	{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f) },// Normal Y -		
		//	{ Vector3(1.0f, -1.0f, -1.0f),	Vector3(0.0f, -1.0f, 0.0f) },
		//	{ Vector3(1.0f, -1.0f, 1.0f),	Vector3(0.0f, -1.0f, 0.0f) },
		//	{ Vector3(-1.0f, -1.0f, 1.0f),	Vector3(0.0f, -1.0f, 0.0f) },

		//	{ Vector3(-1.0f, -1.0f, 1.0f),	Vector3(-1.0f, 0.0f, 0.0f) },//	Normal X -
		//	{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f) },
		//	{ Vector3(-1.0f, 1.0f, -1.0f),	Vector3(-1.0f, 0.0f, 0.0f) },
		//	{ Vector3(-1.0f, 1.0f, 1.0f),	Vector3(-1.0f, 0.0f, 0.0f) },

		//	{ Vector3(1.0f, -1.0f, 1.0f),	Vector3(1.0f, 0.0f, 0.0f) },// Normal X +
		//	{ Vector3(1.0f, -1.0f, -1.0f),	Vector3(1.0f, 0.0f, 0.0f) },
		//	{ Vector3(1.0f, 1.0f, -1.0f),	Vector3(1.0f, 0.0f, 0.0f) },
		//	{ Vector3(1.0f, 1.0f, 1.0f),	Vector3(1.0f, 0.0f, 0.0f) },

		//	{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f) }, // Normal Z -
		//	{ Vector3(1.0f, -1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f) },
		//	{ Vector3(1.0f, 1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f) },
		//	{ Vector3(-1.0f, 1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f) },

		//	{ Vector3(-1.0f, -1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f) },// Normal Z +
		//	{ Vector3(1.0f, -1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f) },
		//	{ Vector3(1.0f, 1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f) },
		//	{ Vector3(-1.0f, 1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f) },
		//};
	}

	// Tangent 값은 uv좌표계에서 면에 대한 u값으로 설정
	//Vertex vertices[] =
	//{
	//	// 윗면
	//	{ Vector3(-1.0f, 1.0f, -1.0f), Vector2(1.0f, 0.0f),	Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)},
	//	{ Vector3(1.0f, 1.0f, -1.0f), Vector2(0.0f, 0.0f),	Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)},
	//	{ Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f),	Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)},
	//	{ Vector3(-1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f),	Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)},

	//	// 아랫면
	//	{ Vector3(-1.0f, -1.0f, -1.0f), Vector2(0.0f, 0.0f),Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f) },
	//	{ Vector3(1.0f, -1.0f, -1.0f), Vector2(1.0f, 0.0f),	Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f) },
	//	{ Vector3(1.0f, -1.0f, 1.0f), Vector2(1.0f, 1.0f),	Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f) }	,
	//	{ Vector3(-1.0f, -1.0f, 1.0f), Vector2(0.0f, 1.0f),	Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f) },

	//	// 왼쪽면
	//	{ Vector3(-1.0f, -1.0f, 1.0f), Vector2(0.0f, 1.0f),	Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f)	},
	//	{ Vector3(-1.0f, -1.0f, -1.0f), Vector2(1.0f, 1.0f),Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f)	},
	//	{ Vector3(-1.0f, 1.0f, -1.0f), Vector2(1.0f, 0.0f),	Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f)	},
	//	{ Vector3(-1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f),	Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f)	},

	//	// 오른쪽면
	//	{ Vector3(1.0f, -1.0f, 1.0f), Vector2(1.0f, 1.0f),	Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)	},
	//	{ Vector3(1.0f, -1.0f, -1.0f), Vector2(0.0f, 1.0f),	Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)	},
	//	{ Vector3(1.0f, 1.0f, -1.0f), Vector2(0.0f, 0.0f),	Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)	},
	//	{ Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f),	Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)	},

	//	// 앞면
	//	{ Vector3(-1.0f, -1.0f, -1.0f), Vector2(0.0f, 1.0f),Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f)	},
	//	{ Vector3(1.0f, -1.0f, -1.0f), Vector2(1.0f, 1.0f),	Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f)	},
	//	{ Vector3(1.0f, 1.0f, -1.0f), Vector2(1.0f, 0.0f),	Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f)	},
	//	{ Vector3(-1.0f, 1.0f, -1.0f), Vector2(0.0f, 0.0f),	Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f)	},

	//	// 뒷면
	//	{ Vector3(-1.0f, -1.0f, 1.0f), Vector2(1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f) },
	//	{ Vector3(1.0f, -1.0f, 1.0f), Vector2(0.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f) },
	//	{ Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f),	Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f) },
	//	{ Vector3(-1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f),	Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f) },
	//};

	//D3D11_BUFFER_DESC vbDesc = {};
	//vbDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);;	// 생성될 버텍스 버퍼의 바이트 크기 설정
	//vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// 생성된 버텍스 버퍼를 어떻게 바인딩 할지
	//vbDesc.Usage = D3D11_USAGE_DEFAULT;					// 버퍼의 사용 방법 설정
	//vbDesc.CPUAccessFlags = 0;							// CPU에서 버퍼 데이터에 엑세스 하는 방식을 설정, 0이기 때문에 엑세스 X
	//vbDesc.MiscFlags = 0;								// 추가적인 버퍼 옵션

	//// 정점 버퍼 생성.
	//D3D11_SUBRESOURCE_DATA vbData = {};
	//vbData.pSysMem = vertices;
	//HR_T(hr = m_pDevice->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer));

	//// 버텍스 버퍼 정보 
	//m_VertexBufferStride = sizeof(Vertex);
	//m_VertexBufferOffset = 0;

	// 정점 정보를 넘길때 추가하는 정보되는 정보들을 여기다 넣어줘야함
	// 정점들을 생산할 때 참고하는 계획서
	D3D11_INPUT_ELEMENT_DESC layout[] =  // 인풋 레이아웃은 버텍스 쉐이더가 입력받을 데이터의 형식을 지정한다.
	{// SemanticName , SemanticIndex , Format , InputSlot , AlignedByteOffset , InputSlotClass , InstanceDataStepRate		
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		//{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ID3DBlob* vertexShaderBuffer = nullptr;
	HR_T(CompileShaderFromFile(L"BasicVertexShader.hlsl", "main", "vs_5_0", &vertexShaderBuffer));
	HR_T(hr = m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout));
	// 2. Render() 에서 파이프라인에 바인딩할 InputLayout 생성 


	// 3. Render에서 파이프라인에 바인딩할  버텍스 셰이더 생성
	HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader));
	SAFE_RELEASE(vertexShaderBuffer);

	// 4. Render에서 파이프라인에 바인딩할 인덱스 버퍼 생성
	/*WORD indices[] =
	{
		1,2,0, 3,2,1,
		3,5,7, 1,5,3,
		1,4,5, 0,4,1,
		2,7,6, 3,7,2,
		0,6,4, 2,6,0,
		4,7,5, 6,7,4
	};*/

	//WORD indices[] =
	//{
	//	3,1,0, 2,1,3,
	//	6,4,5, 7,4,6,
	//	11,9,8, 10,9,11,
	//	14,12,13, 15,12,14,
	//	19,17,16, 18,17,19,
	//	22,20,21, 23,20,22
	//};

	//m_nIndices = ARRAYSIZE(indices);	// 인덱스 개수 저장.
	//vbDesc = {};
	//vbDesc.ByteWidth = sizeof(WORD) * ARRAYSIZE(indices);
	//vbDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//vbDesc.Usage = D3D11_USAGE_DEFAULT;

	//D3D11_SUBRESOURCE_DATA ibData = {};
	//ibData.pSysMem = indices;
	//HR_T(m_pDevice->CreateBuffer(&vbDesc, &ibData, &m_pIndexBuffer));

	// 5. Render() 에서 파이프라인에 바인딩할 픽셀 셰이더 생성
	ID3DBlob* pixelShaderBuffer = nullptr;
	HR_T(CompileShaderFromFile(L"BasicPixelShader.hlsl", "main", "ps_5_0", &pixelShaderBuffer));
	HR_T(m_pDevice->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
	SAFE_RELEASE(pixelShaderBuffer);

	//HR_T(CompileShaderFromFile(L"SolidPixelShader.hlsl", "main", "ps_5_0", &pixelShaderBuffer));
	//HR_T(m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
	//	pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShaderSolid));
	//SAFE_RELEASE(pixelShaderBuffer);

	// 6. Render() 에서 파이프라인에 바인딩할 상수 버퍼 생성
// Create the constant buffer
	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.ByteWidth = sizeof(ConstantBuffer);
	vbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&vbDesc, nullptr, &m_pConstantBuffer));

	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.ByteWidth = sizeof(CB_Lighting);
	vbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&vbDesc, nullptr, &m_pCB_Lighting));

	// 나중에 셰이더 따로 만들어서 DDS파일 로드하는것도 다시 만들자
	//m_ResourceLoader.CreateTextureFromFile(m_pDevice, L"Sanfrancisco.dds", &m_pTextureRV);
	
	// Sample State
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR_T(m_pDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear));

	// 쉐이더에 전달할 데이터 설정
	// Initialize the world matrix

	
	/*m_ResourceLoader.CreateTextureFromFile(m_pDevice, L"Bricks059_1K-JPG_Color.jpg", &m_pTextureRV);
	m_ResourceLoader.CreateTextureFromFile(m_pDevice, L"Bricks059_1K-JPG_NormalDX.jpg", &m_pNormalMap);
	m_ResourceLoader.CreateTextureFromFile(m_pDevice, L"Bricks059_Specular.png", &m_pSpecularMap);*/

	ResourceLoader loader;

	m_Model.push_back(loader.LoadModel(m_pDevice, "../res/models/BoxHuman.fbx"));

	return true;
}

void MyEngine::UnInitScene()
{
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pCB_Lighting);

	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pDepthStencilView);
}

bool MyEngine::InitImGUI()
{	/*
		ImGui 초기화.
	*/
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// Setup Dear ImGui style
	ImGui::StyleColorsLight();
	//ImGui::StyleColorsLight();
	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(this->m_pDevice, this->m_pDeviceContext);

	//
	return true;
}

void MyEngine::UnInitImGUI()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT MyEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	return __super::WndProc(hWnd, message, wParam, lParam);
}
