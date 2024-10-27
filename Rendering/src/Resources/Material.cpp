#include "Resources/Material.h"

Material::Material() {}
Material::~Material() {}

void Material::Bind()
{
    for (size_t i = 0; i < texturesIDs.size(); ++i)
    {
        // Get Texture UUID
        // Fetch from Asset Manager Texture
        // Bind to slot i
    }
}

void Material::Unbind()
{
    for (size_t i = 0; i < texturesIDs.size(); ++i)
    {
        // Get Texture UUID
        // Fetch from Asset Manager Texture
        // Unbind
    }
}
