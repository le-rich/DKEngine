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
	UUIDv4::UUID mRoughnessMapID;

    Material();
	Material(const UUIDv4::UUID& pShaderID, const UUIDv4::UUID& pBaseColorTextureID);
    ~Material();

    void Bind();
    void Unbind();

private:
    std::vector<UUIDv4::UUID> texturesIDs;

};

