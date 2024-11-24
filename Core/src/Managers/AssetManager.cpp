#include "Managers/AssetManager.h"

void AssetManager::AddMaterial(std::shared_ptr<Material> pMaterial)
{
    auto result = mMaterialMap.insert({ pMaterial->GetAssetID(), pMaterial });
}

void AssetManager::AddShader(std::shared_ptr<Shader> pShader)
{
    auto result = mShaderMap.insert({ pShader->GetAssetID(), pShader });
}

void AssetManager::AddSkybox(std::shared_ptr<Skybox> pSkybox)
{
    auto result = mSkyboxMap.insert({ pSkybox->GetAssetID(), pSkybox });
}

void AssetManager::AddTexture(std::shared_ptr<Texture> pTexuture)
{
    auto result = mTextureMap.insert({ pTexuture->GetAssetID(), pTexuture });
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

std::shared_ptr<Skybox> AssetManager::GetSkyboxByID(UUIDv4::UUID& pID)
{
    auto result = mSkyboxMap.find(pID);

    if (result != mSkyboxMap.end())
    {
        return result->second;
    }

    std::cerr << "Error: Skybox with UUID " << pID << " not found in the map." << std::endl;
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

std::shared_ptr<Skybox> AssetManager::GetSkyboxByName(std::string pName)
{
    for (auto it = mSkyboxMap.begin(); it != mSkyboxMap.end(); ++it)
    {
        UUIDv4::UUID uuid = it->first;
        std::shared_ptr<Skybox> skybox = it->second;

        if (skybox->GetDisplayName() == pName)
        {
            return it->second;
        }
    }

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

void AssetManager::RemoveSkyboxByID(UUIDv4::UUID& pID)
{
    auto it = mSkyboxMap.find(pID);
    if (it != mSkyboxMap.end())
    {
        mSkyboxMap.erase(it);
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