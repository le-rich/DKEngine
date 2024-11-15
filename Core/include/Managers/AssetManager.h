#pragma once
#include <map>

#include "Utils/IDUtils.h"
#include "Resources/Material.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

static const std::string DEFAULT_SHADER_PATH = "../Rendering/Shaders/default.glsl";
static const std::string DEFAULT_ASSET_FOLDER = "Assets/";

class AssetManager
{
public:
    // default constructor
    static AssetManager& GetInstance()
    {
        static AssetManager instance;
        return instance;
    }

    std::shared_ptr<Shader> GetDefaultShader() { return DEFAULT_SHADER; }

    void AddMaterial(std::shared_ptr<Material> pMaterial);
    void AddShader(std::shared_ptr<Shader> pShader);
    void AddTexture(std::shared_ptr<Texture> pTexture);

    std::shared_ptr<Material> GetMaterialByID(UUIDv4::UUID& pID);
    std::shared_ptr<Shader> GetShaderByID(UUIDv4::UUID& pID);
    std::shared_ptr<Texture> GetTextureByID(const UUIDv4::UUID& pID);

    // removes an entity from the map by UUID
    void RemoveMaterialByID(UUIDv4::UUID& pID);
    void RemoveShaderByID(UUIDv4::UUID& pID);
    void RemoveTextureByID(UUIDv4::UUID& pID);

private:
    std::shared_ptr<Shader> DEFAULT_SHADER = std::make_shared<Shader>(DEFAULT_SHADER_PATH, "default");

    std::map<UUIDv4::UUID, std::shared_ptr<Material>> mMaterialMap;
    std::map<UUIDv4::UUID, std::shared_ptr<Shader>> mShaderMap;
    std::map<UUIDv4::UUID, std::shared_ptr<Texture>> mTextureMap;

    // Private constructor as this should be a singleton.
    AssetManager()
    {
        mShaderMap.insert({ DEFAULT_SHADER->GetAssetID(), DEFAULT_SHADER });
        std::cout << "Asset Manager created" << std::endl;
    }

public:
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
};