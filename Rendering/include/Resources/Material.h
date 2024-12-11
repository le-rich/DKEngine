#pragma once
#include "Resources/Asset.h"

#include <glm.hpp>
#include <string>

// TEXTURE MAP ORDER
// Diffuse map
// Metallic map
// Normal map
// Height map
// AmbientOcclusion map
// Roughness map
static const int PBR_SUPPORTED_TEXTURE_COUNT = 6;

class Material : public Asset
{
public:
    UUIDv4::UUID mShaderID;

    Material();
    Material(const UUIDv4::UUID& pShaderID, const UUIDv4::UUID& pBaseColorTextureID);
    ~Material();

    void Bind();
    void Unbind();

    inline void SetTexture(int pSlot, const UUIDv4::UUID& pTextureID) { texturesIDs[pSlot] = pTextureID; }
    inline UUIDv4::UUID& GetTexture(int pSlot) { return texturesIDs[pSlot]; }

private:
    UUIDv4::UUID texturesIDs[PBR_SUPPORTED_TEXTURE_COUNT];

};

