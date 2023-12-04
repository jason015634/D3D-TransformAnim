#pragma once
#include "ConstantBuffers.h"
#include <d3d11.h>
#include <string>
#include <filesystem>
struct aiMaterial;

class Model;
class Material
{
public:
    Material(Model* _model);
    ~Material();

    Model* m_pOwner;

private:
    std::wstring FBXFileName;

public:

    Vector4 m_baseColor = {};
    CB_Lighting m_materialCB;
    ID3D11ShaderResourceView* m_DiffuseRV = nullptr;        // �ؽ�ó ���ҽ� ��
    ID3D11ShaderResourceView* m_NormalRV = nullptr;         // �ؽ�ó �븻�� ���ҽ� ��
    ID3D11ShaderResourceView* m_SpecularRV = nullptr;       // �ؽ�ó ����ŧ���� ���ҽ� ��
    ID3D11ShaderResourceView* m_EmissiveRV = nullptr;       // �ؽ�ó �̹̽ú�� ���ҽ� ��
    ID3D11ShaderResourceView* m_OpacityRV = nullptr;        // �ؽ�ó ���Ľ�Ƽ�� ���ҽ� ��

public:
    void Create(ID3D11Device* device, aiMaterial* material);
    void SetFileName(const std::wstring& fileName);

    void Render(ID3D11DeviceContext* _dc, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB);
};

