#include "pch.h"
#include "Material.h"

#include <Directxtk/WICTextureLoader.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <filesystem>
#include "ResourceLoader.h"
using namespace DirectX;
Material::Material(Model* _model)
    :m_pOwner(_model)
{
}

Material::~Material()
{
    SAFE_RELEASE(m_DiffuseRV);
    SAFE_RELEASE(m_NormalRV);
    SAFE_RELEASE(m_SpecularRV);
    SAFE_RELEASE(m_EmissiveRV);
    SAFE_RELEASE(m_OpacityRV);
}

void Material::Create(ID3D11Device* device, aiMaterial* material)
{
    // 경로 설정
    aiString texturePath;
    std::filesystem::path path;
    std::wstring folderPath;
    std::string name = material->GetName().C_Str();
    std::wstring basePath = L"../res/textures";

    for (const auto& entry : std::filesystem::recursive_directory_iterator(basePath))
    {
        if (entry.is_regular_file() && (entry.path().extension() == L".png" || entry.path().extension() == L".jpg"))
        {
            // fbx 이름이랑 폴더 이름이랑 같으면 folderPath 생성
            folderPath = entry.path().parent_path().wstring();

            std::wstring extractedString;
            size_t lastSlash = folderPath.find_last_of('\\');
            size_t lastDot = folderPath.find_last_of('.');

            if (lastSlash != std::string::npos && lastDot != std::string::npos)
            {
                extractedString = folderPath.substr(lastSlash + 1, lastDot - lastSlash - 1);
            }

            if (FBXFileName == extractedString)
            {
                name = material->GetName().C_Str();
                break;
            }
        }
    }

    // 텍스처 로드

    aiColor3D color(0.f, 0.f, 0.f);
    if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
    {
        m_baseColor = { color.r,color.g,color.b,1.f };
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        HR_T(ResourceLoader::CreateTextureFromFile(device, finalPath.c_str(), &m_DiffuseRV));
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_NORMALS, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        HR_T(ResourceLoader::CreateTextureFromFile(device, finalPath.c_str(), &m_NormalRV));
        //HR_T(DirectX::CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_NormalRV));
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_SPECULAR, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        HR_T(DirectX::CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_SpecularRV));
        //HR_T(DirectX::CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_SpecularRV));
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        HR_T(DirectX::CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_EmissiveRV));
        //HR_T(DirectX::CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_EmissiveRV));
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_OPACITY, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        HR_T(DirectX::CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_OpacityRV));
        //HR_T(DirectX::CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_OpacityRV));
    }
}

void Material::SetFileName(const std::wstring& fileName)
{
    FBXFileName = fileName;
}

void Material::Render(ID3D11DeviceContext* _dc, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB)
{
    m_materialCB.UseDiffuseMap = m_DiffuseRV != nullptr ? true : false;
    m_materialCB.UseNormalMap = m_NormalRV != nullptr ? true : false;
    m_materialCB.UseSpecularMap = m_SpecularRV != nullptr ? true : false;
    m_materialCB.UseEmissiveMap = m_EmissiveRV != nullptr ? true : false;
    m_materialCB.UseOpacityMap = m_OpacityRV != nullptr ? true : false;


    _dc->PSSetShaderResources(0, 1, &m_DiffuseRV);
    _dc->PSSetShaderResources(1, 1, &m_NormalRV);
    _dc->PSSetShaderResources(2, 1, &m_SpecularRV);
    _dc->PSSetShaderResources(3, 1, &m_EmissiveRV);
    _dc->PSSetShaderResources(4, 1, &m_OpacityRV);

    if (m_materialCB.UseOpacityMap)
        _dc->OMSetBlendState(m_pAlphaBlendState, nullptr, 0xffffffff); // 알파블렌드 상태설정 , 다른옵션은 기본값 
    else
        _dc->OMSetBlendState(nullptr, nullptr, 0xffffffff);	// 설정해제 , 다른옵션은 기본값

    _dc->UpdateSubresource(m_pMaterialCB, 0, nullptr, &m_materialCB, 0, 0);
}

