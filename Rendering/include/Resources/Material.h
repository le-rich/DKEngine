#pragma once
#include "Resources/Asset.h"

class Material : public Asset
{
public:
    UUIDv4::UUID mBaseColorTextureID;
    UUIDv4::UUID mShaderID;

    Material();
    ~Material();
    void Bind();
    void Unbind();
private:
    std::vector<UUIDv4::UUID> texturesIDs;
};

