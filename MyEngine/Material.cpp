#include "../GameApp/pch.h"
#include "Material.h"

#include "../GameApp/Helper.h"
#include <Directxtk/WICTextureLoader.h>
#include <filesystem>
#include <assimp/scene.h>
#include <assimp/types.h>

using namespace DirectX;

Material::Material()
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
    std::string name;
    std::wstring basePath = L"../Resource/FBXLoad_Test/texture";

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
    if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        HR_T(DirectX::CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_DiffuseRV));
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_NORMALS, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        HR_T(DirectX::CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_NormalRV));
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_SPECULAR, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        HR_T(DirectX::CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_SpecularRV));
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        HR_T(DirectX::CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_EmissiveRV));
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_OPACITY, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        HR_T(DirectX::CreateWICTextureFromFile(device, finalPath.c_str(), nullptr, &m_OpacityRV));
    }
}

void Material::SetFileName(const std::wstring& fileName)
{
    FBXFileName = fileName;
}

