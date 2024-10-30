#include "Managers/AssetManager.h"

void AssetManager::AddMaterial(std::shared_ptr<Material> pMaterial)
{
    auto result = mMaterialMap.insert({ pMaterial->GetAssetID(), pMaterial });

    if (result.second)
    {
        std::cout << "Material added" << std::endl;
    }
}

void AssetManager::AddShader(std::shared_ptr<Shader> pShader)
{
    auto result = mShaderMap.insert({ pShader->GetAssetID(), pShader });

    if (result.second)
    {
        std::cout << "Shader added" << std::endl;
    }
}

void AssetManager::AddTexture(std::shared_ptr<Texture> pTexuture)
{
    auto result = mTextureMap.insert({ pTexuture->GetAssetID(), pTexuture });

    if (result.second)
    {
        std::cout << "Texture added" << std::endl;
    }
}

std::shared_ptr<Material> AssetManager::GetMaterialByID(UUIDv4::UUID& pID)
{
    auto result = mMaterialMap.find(pID);

    if (result != mMaterialMap.end())
    {
        return result->second;
    }

    std::cerr << "Error: Material with UUID " << pID << " not found in the map." << std::endl;
    return nullptr;
}

std::shared_ptr<Shader> AssetManager::GetShaderByID(UUIDv4::UUID& pID)
{
    auto result = mShaderMap.find(pID);

    if (result != mShaderMap.end())
    {
        return result->second;
    }

    std::cerr << "Error: Shader with UUID " << pID << " not found in the map." << std::endl;
    return nullptr;
}

std::shared_ptr<Texture> AssetManager::GetTextureByID(const UUIDv4::UUID& pID)
{
    auto result = mTextureMap.find(pID);

    if (result != mTextureMap.end())
    {
        return result->second;
    }

    std::cerr << "Error: Texture with UUID " << pID << " not found in the map." << std::endl;
    return nullptr;
}

void AssetManager::RemoveMaterialByID(UUIDv4::UUID& pID)
{
    auto it = mMaterialMap.find(pID);
    if (it != mMaterialMap.end())
    {
        mMaterialMap.erase(it);
    }
}

void AssetManager::RemoveShaderByID(UUIDv4::UUID& pID)
{
    auto it = mShaderMap.find(pID);
    if (it != mShaderMap.end())
    {
        mShaderMap.erase(it);
    }
}

void AssetManager::RemoveTextureByID(UUIDv4::UUID& pID)
{
    auto it = mTextureMap.find(pID);
    if (it != mTextureMap.end())
    {
        mTextureMap.erase(it);
    }
}