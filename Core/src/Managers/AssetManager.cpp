#include "Managers/AssetManager.h"

void AssetManager::AddTexture(std::shared_ptr<Texture> pTexuture)
{
    auto result = mTextureMap.insert({ pTexuture->GetAssetID(), pTexuture });

    if (result.second)
    {
        std::cout << "Texture added" << std::endl;
    }
}

std::shared_ptr<Texture> AssetManager::GetTextureByID(const UUIDv4::UUID& pID)
{
    auto result = mTextureMap.find(pID);

    if (result != mTextureMap.end())
    {
        return result->second;
    }

    std::cerr << "Error: Entity with UUID " << pID << " not found in the map." << std::endl;
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