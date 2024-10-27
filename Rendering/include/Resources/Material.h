#pragma once
#include "Resources/Asset.h"

class Material : public Asset
{
public:
    Material();
    ~Material();
    void Bind();
    void Unbind();

private:
    std::vector<UUIDv4::UUID> texturesIDs;
    UUIDv4::UUID Shader;
};

