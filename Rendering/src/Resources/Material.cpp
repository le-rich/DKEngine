#include "Resources/Material.h"
#include "Managers/AssetManager.h"

Material::Material() {}
Material::~Material() {}

void Material::Bind()
{
    AssetManager::GetInstance().GetTextureByID(mBaseColorTextureID)->Bind();
    for (size_t i = 0; i < texturesIDs.size(); ++i)
    {
        // Get Texture UUID
        // Fetch from Asset Manager Texture
        // Bind to slot i
    }
}

void Material::Unbind()
{
    AssetManager::GetInstance().GetTextureByID(mBaseColorTextureID)->Unbind();
    for (size_t i = 0; i < texturesIDs.size(); ++i)
    {
        // Get Texture UUID
        // Fetch from Asset Manager Texture
        // Unbind
    }
}
