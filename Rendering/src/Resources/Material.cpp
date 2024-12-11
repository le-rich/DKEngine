#include "Resources/Material.h"
#include "Managers/AssetManager.h"

Material::Material() {}
Material::Material(const UUIDv4::UUID& pShaderID, const UUIDv4::UUID& pBaseColorTextureID)
{
	mShaderID = pShaderID;
	mBaseColorTextureID = pBaseColorTextureID;

}

Material::~Material() {}

void Material::Bind()
{
    AssetManager::GetInstance().GetTextureByID(mBaseColorTextureID)->Bind();
    AssetManager::GetInstance().GetShaderByID(mShaderID)->Use();

    std::shared_ptr<Texture> texture = AssetManager::GetInstance().GetTextureByID(mMetallicMapID);
    if (texture != nullptr) 
    {
        texture->BindToSlot(1);
    }

    texture = AssetManager::GetInstance().GetTextureByID(mNormalMapID);
    if (texture != nullptr) 
    {
        texture->BindToSlot(2);
    }

    texture = AssetManager::GetInstance().GetTextureByID(mHeightMapID);
    if (texture != nullptr) 
    {
        texture->BindToSlot(3);
    }

    texture = AssetManager::GetInstance().GetTextureByID(mAmbientOcclusionMapID);
    if (texture != nullptr) 
    {
        texture->BindToSlot(4);
    }

    texture = AssetManager::GetInstance().GetTextureByID(mRoughnessMapID);
    if (texture != nullptr) 
    {
        texture->BindToSlot(5);
    }

    for (size_t i = 0; i < texturesIDs.size(); ++i)
    {
        // Get Texture UUID
        // Fetch from Asset Manager Texture
        // Bind to slot i
        //AssetManager::GetInstance().GetTextureByID(texturesIDs.at(i))->BindToSlot(i);
    }
}

void Material::Unbind()
{
    AssetManager::GetInstance().GetTextureByID(mBaseColorTextureID)->Unbind();
    //for (size_t i = 0; i < texturesIDs.size(); ++i)
    //{
    //    // Get Texture UUID
    //    // Fetch from Asset Manager Texture
    //    // Unbind       
    //    AssetManager::GetInstance().GetTextureByID(texturesIDs.at(i))->Unbind();
    //}
    std::shared_ptr<Texture> texture = AssetManager::GetInstance().GetTextureByID(mMetallicMapID);
    if (texture != nullptr)
    {
        texture->Unbind(1);

    }

    texture = AssetManager::GetInstance().GetTextureByID(mNormalMapID);
    if (texture != nullptr)
    {
        texture->Unbind(2);

    }

    texture = AssetManager::GetInstance().GetTextureByID(mHeightMapID);
    if (texture != nullptr)
    {
        texture->Unbind(3);

    }

    texture = AssetManager::GetInstance().GetTextureByID(mAmbientOcclusionMapID);
    if (texture != nullptr)
    {
        texture->Unbind(4);

    }

    texture = AssetManager::GetInstance().GetTextureByID(mRoughnessMapID);
    if (texture != nullptr)
    {
        texture->Unbind(5);

    }


    texturesIDs.clear();
}
