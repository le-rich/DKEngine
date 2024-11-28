#pragma once
#include <map>

#include "Utils/IDUtils.h"
#include "Resources/Material.h"
#include "Resources/Shader.h"
#include "Resources/Skybox.h"
#include "Resources/Texture.h"

static const std::string DEFAULT_SHADER_PATH = "../Rendering/Shaders/default.glsl";
static const std::string EQUIRECT_CUBEMAP_SHADER_PATH = "../Rendering/Shaders/equirectangularToCubemapShader.glsl";
static const std::string SKYBOX_SHADER_PATH = "../Rendering/Shaders/skybox.glsl";
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
    std::shared_ptr<Shader> GetEquirectShader() { return EQUIRECT_CUBEMAP_SHADER; }
    std::shared_ptr<Shader> GetSkyboxShader() { return SKYBOX_SHADER; }

    void AddMaterial(std::shared_ptr<Material> pMaterial);
    void AddShader(std::shared_ptr<Shader> pShader);
    void AddSkybox(std::shared_ptr<Skybox> pSkybox);
    void AddTexture(std::shared_ptr<Texture> pTexture);

    std::shared_ptr<Material> GetMaterialByID(UUIDv4::UUID& pID);
    std::shared_ptr<Shader> GetShaderByID(UUIDv4::UUID& pID);
    std::shared_ptr<Skybox> GetSkyboxByID(UUIDv4::UUID& pID);
    std::shared_ptr<Texture> GetTextureByID(const UUIDv4::UUID& pID);


    std::shared_ptr<Skybox> GetSkyboxByName(std::string pName);
    
    // removes an entity from the map by UUID
    void RemoveMaterialByID(UUIDv4::UUID& pID);
    void RemoveShaderByID(UUIDv4::UUID& pID);
    void RemoveSkyboxByID(UUIDv4::UUID& pID);
    void RemoveTextureByID(UUIDv4::UUID& pID);

private:
    std::shared_ptr<Shader> DEFAULT_SHADER = std::make_shared<Shader>(DEFAULT_SHADER_PATH, "default");
    std::shared_ptr<Shader> EQUIRECT_CUBEMAP_SHADER = std::make_shared<Shader>(EQUIRECT_CUBEMAP_SHADER_PATH, "equirectCubemap");
    std::shared_ptr<Shader> SKYBOX_SHADER = std::make_shared<Shader>(SKYBOX_SHADER_PATH, "skybox");

    std::map<UUIDv4::UUID, std::shared_ptr<Material>> mMaterialMap;
    std::map<UUIDv4::UUID, std::shared_ptr<Shader>> mShaderMap;
    std::map<UUIDv4::UUID, std::shared_ptr<Skybox>> mSkyboxMap;
    std::map<UUIDv4::UUID, std::shared_ptr<Texture>> mTextureMap;

    // Private constructor as this should be a singleton.
    AssetManager()
    {
        mShaderMap.insert({ DEFAULT_SHADER->GetAssetID(), DEFAULT_SHADER });
        mShaderMap.insert({ SKYBOX_SHADER->GetAssetID(), SKYBOX_SHADER });
    }

public:
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
};