#pragma once
#include "Resources/Asset.h"

#include <glm.hpp>
#include <string>

class Material : public Asset
{
public:
    UUIDv4::UUID mShaderID;

    UUIDv4::UUID mBaseColorTextureID;
	UUIDv4::UUID mMetallicMapID;
	UUIDv4::UUID mNormalMapID;
	UUIDv4::UUID mHeightMapID;
	UUIDv4::UUID mAmbientOcclusionMapID;
	UUIDv4::UUID mEmissiveMapID;

    Material();
	Material(const UUIDv4::UUID& pShaderID, const UUIDv4::UUID& pBaseColorTextureID);
    ~Material();

    void Bind();
    void Unbind();

	// Underlying defaults if not overwritten by a map.
    glm::vec4 albedoColor = glm::vec4(1.0f); // RGBA
    float metallic = 0.0f;
    float roughness = 0.5f;
    glm::vec3 emission = glm::vec3(1.0f);    // RGB

private:
    std::vector<UUIDv4::UUID> texturesIDs;

};

