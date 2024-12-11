#include "Resources/Material.h"
#include "Managers/AssetManager.h"

Material::Material() {}
Material::Material(const UUIDv4::UUID& pShaderID, const UUIDv4::UUID& pBaseColorTextureID)
{
	mShaderID = pShaderID;
    texturesIDs[0] = pBaseColorTextureID;
}

Material::~Material() {}

void Material::Bind()
{
    AssetManager::GetInstance().GetShaderByID(mShaderID)->Use();

    // Get Texture UUID
    // Fetch from Asset Manager Texture
    // Bind to slot i
    for (size_t i = 0; i < PBR_SUPPORTED_TEXTURE_COUNT; ++i)
    {
        auto texture = AssetManager::GetInstance().GetTextureByID(texturesIDs[i]);
        if (texture == nullptr) continue;
        texture->BindToSlot(i);
    }
}

void Material::Unbind()
{
    for (size_t i = 0; i < PBR_SUPPORTED_TEXTURE_COUNT; ++i)
    {
        auto texture = AssetManager::GetInstance().GetTextureByID(texturesIDs[i]);
        if (texture == nullptr) continue;
        texture->Unbind(i);
    }
}
