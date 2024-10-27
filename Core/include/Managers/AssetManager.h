#pragma once
#include <map>

#include "Utils/IDUtils.h"
#include "Resources/Material.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

class AssetManager
{
public:
    // default constructor
    static AssetManager& GetInstance()
    {
        static AssetManager instance;
        return instance;
    }

    void AddMaterial(std::shared_ptr<Material> pMaterial);
    //void AddShader();
    void AddTexture(std::shared_ptr<Texture> pTexture);

    std::shared_ptr<Material> GetMaterialByID(UUIDv4::UUID& pID);
    //Shader GetShaderByID(UUIDv4::UUID pID);
    std::shared_ptr<Texture> GetTextureByID(const UUIDv4::UUID& pID);

    // removes an entity from the map by UUID
    void RemoveMaterialByID(UUIDv4::UUID& eID);
    void RemoveShaderByID(UUIDv4::UUID& eID);
    void RemoveTextureByID(UUIDv4::UUID& eID);

private:
    std::map<UUIDv4::UUID, std::shared_ptr<Material>> mMaterialMap;
    std::map<UUIDv4::UUID, std::shared_ptr<Shader>> mShaderMap;
    std::map<UUIDv4::UUID, std::shared_ptr<Texture>> mTextureMap;

    // Private constructor as this should be a singleton.
    AssetManager()
    {
        std::cout << "Asset Manager created" << std::endl;
    }

public:
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
};