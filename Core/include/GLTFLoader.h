#pragma once
#include "Components/MeshComponent.h"
#include "Components/RigidbodyComponent.h"
#include "Managers/AssetManager.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "Resources/Texture.h"

#include <tiny_gltf.cc>
#include <glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <regex>

namespace GLTFLoader
{
    /*
    * Return the count of the attribute with the highest count
    */
    static const int GetMaxElementCount(tinygltf::Model pGltfModel, tinygltf::Primitive pPrimitive)
    {
        int maxCount = 0;
        for (const auto& attribute : pPrimitive.attributes)
        {
            const int accessorNum = attribute.second;
            const tinygltf::Accessor& accessor = pGltfModel.accessors[accessorNum];

            if (maxCount < accessor.count)
            {
                maxCount = accessor.count;
            }
        }
        return maxCount;
    }

    /*
    * Return amount of components in the given GLTF data type
    */
    int GetNumberOfComponentsInType(size_t pDataType)
    {
        switch (pDataType)
        {
        case TINYGLTF_TYPE_SCALAR:
            return 1;
        case TINYGLTF_TYPE_VEC2:
            return 2;
        case TINYGLTF_TYPE_VEC3:
            return 3;
        default:
            std::printf("Unsupported data type in gltf model!");
            return 0;
        }
    }

    /*
    * Return the number of bytes in the given GLTF component type
    */
    static int GetComponentsSize(size_t pComponentType)
    {
        switch (pComponentType)
        {
        case TINYGLTF_COMPONENT_TYPE_BYTE:
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
            return 1;
        case TINYGLTF_COMPONENT_TYPE_SHORT:
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            return 2;
        case TINYGLTF_COMPONENT_TYPE_INT:
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
        case TINYGLTF_COMPONENT_TYPE_FLOAT:
            return 4;
        default:
            std::printf("Unsupported data component type in gltf model!");
        }
    }

    /*
    * Fills OutBufferData with the data from the Buffer view specified at the specified accessor
    *
    * @param
    * pGltfModel: GLTF model data
    *
    * pAccessorNum: Index of the specefied accessor
    *
    * OutBufferData: Target buffer to fill
    */
    static bool GetAttributeVector(tinygltf::Model const& pGltfModel, uint32_t const pAccessorNum, std::vector<unsigned char>& OutBufferData)
    {
        const tinygltf::Accessor& accessor = pGltfModel.accessors[pAccessorNum];
        const tinygltf::BufferView& bufferView = pGltfModel.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = pGltfModel.buffers[bufferView.buffer];

        int typeCount = GetNumberOfComponentsInType(accessor.type);

        size_t componentType = accessor.componentType;
        size_t componentSize = GetComponentsSize(componentType);

        int numberOfElements = accessor.count;
        for (int element = 0; element < numberOfElements; ++element)
        {
            auto startShift = bufferView.byteOffset + (element * bufferView.byteStride) + (element * componentSize * typeCount);
            auto start = buffer.data.begin() + startShift;
            auto endShift = startShift + (componentSize * typeCount);
            auto end = buffer.data.begin() + endShift;

            OutBufferData.insert(OutBufferData.end(), start, end);
        }
        return true;
    }

    /*
    * Returns primitive object
    */
    static Primitive ProcessPrimitive(tinygltf::Model pGltfModel, tinygltf::Primitive pPrimitive, std::vector<UUIDv4::UUID>& materials)
    {
        std::vector<Vertex> vertices;
        int numOfElements = GetMaxElementCount(pGltfModel, pPrimitive);
        vertices.resize(numOfElements);
        for (const auto& attribute : pPrimitive.attributes)
        {
            const std::string attribType = attribute.first;
            const int accessorNum = attribute.second;

            std::vector<unsigned char> bufferData;
            if (!GetAttributeVector(pGltfModel, accessorNum, bufferData))
            {
                std::printf("Failed to get attribute data\n");
                continue;
            }

            // gltf format dictates Little Endian format
            std::vector<float> floatVector(bufferData.size());
            std::memcpy(floatVector.data(), bufferData.data(), bufferData.size());

            // Get number of elements that make up component
            const int typeCount = GetNumberOfComponentsInType(pGltfModel.accessors[accessorNum].type);

            // TODO: Modify to support multi texturing
            switch (GetVertexAttributeFromString(attribType))
            {
            case VertexAttributeKeys::POSITION:
                for (int index = 0; index < vertices.size(); ++index)
                {
                    const int floatIndex = index * typeCount; // Offset
                    vertices[index].mPosition = glm::vec3(floatVector[floatIndex], floatVector[floatIndex + 1], floatVector[floatIndex + 2]);
                }
                break;
            case VertexAttributeKeys::NORMAL:
                for (int index = 0; index < vertices.size(); ++index)
                {
                    int floatIndex = index * typeCount; // Offset
                    vertices[index].mNormal = glm::vec3(floatVector[floatIndex], floatVector[floatIndex + 1], floatVector[floatIndex + 2]);
                }
                break;
            case VertexAttributeKeys::TEXCOORD_0:
                for (int index = 0; index < vertices.size(); ++index)
                {
                    int floatIndex = index * typeCount; // Offset
                    vertices[index].mUVCoord = glm::vec2(floatVector[floatIndex], floatVector[floatIndex + 1]);
                }
                break;
            default:
                printf("Unsupported attribute data type!");
                continue;

            }
        }


        std::vector<uint32_t> indices;
        {
            std::vector<unsigned char> bufferData;
            if (!GetAttributeVector(pGltfModel, pPrimitive.indices, bufferData))
            {
                std::printf("Failed to get attribute data");
            }
            const auto bufferDataSizeBytes = sizeof(unsigned char) * bufferData.size();

            // Convert 2 byte data to uint32_t little-endian
            for (int bufferIndex = 0; bufferIndex < bufferData.size(); bufferIndex += 2)
            {
                uint32_t index = ((uint32_t)bufferData[bufferIndex + 1] << 8 | (uint32_t)bufferData[bufferIndex]);
                indices.push_back(index);
            }
        }

        Primitive primitive(vertices, indices, materials[pPrimitive.material]);
        return primitive;
    }

    /*
    * Returns mesh
    */
    static Mesh* LoadMesh(tinygltf::Model pGltfModel, tinygltf::Mesh pMesh, std::vector<UUIDv4::UUID>& materials)
    {
        Mesh* mesh = new Mesh();
        for (tinygltf::Primitive const& primitive : pMesh.primitives)
            mesh->AddPrimitive(ProcessPrimitive(pGltfModel, primitive, materials));
        return mesh;
    }

    // Load Material
    static std::shared_ptr<Material> LoadMaterial(tinygltf::Material const& pMaterial, std::vector<UUIDv4::UUID> pTextures)
    {
        std::shared_ptr<Material> material = std::make_shared<Material>();

        // Set material properties
        material->mBaseColorTextureID = pTextures[pMaterial.pbrMetallicRoughness.baseColorTexture.index];
        // Set to default shader
        material->mShaderID = AssetManager::GetInstance().GetDefaultShader()->GetAssetID();

        return material;
    }

    // Load Materials into asset manager and return list of id's for this set of textures
    static std::vector<UUIDv4::UUID> LoadMaterials(tinygltf::Model const& pGltfModel, std::vector<UUIDv4::UUID> pTextures)
    {
        std::vector<UUIDv4::UUID> materials(pGltfModel.materials.size());

        for (size_t i = 0; i < materials.size(); ++i)
        {
            tinygltf::Material material = pGltfModel.materials[i];
            std::shared_ptr<Material> materialPointer = LoadMaterial(material, pTextures);
            AssetManager::GetInstance().AddMaterial(materialPointer);
            materials[i] = materialPointer->GetAssetID();
        }

        return materials;
    }

    // TODO: Get and set image sampling properties and pass to Texture object
    static std::shared_ptr<Texture> LoadTexture(tinygltf::Model const& pGltfModel, tinygltf::Texture const& pTexture, std::string const pSourceFolder)
    {
        int imageIndex = pTexture.source;
        tinygltf::Image image = pGltfModel.images[imageIndex];
        std::shared_ptr<Texture> texture = std::make_shared<Texture>(pSourceFolder + image.uri);
        AssetManager::GetInstance().AddTexture(texture);
        return texture;
    }

    // Load Textures into asset manager and return list of id's for this set of textures
    static std::vector<UUIDv4::UUID> LoadTextures(tinygltf::Model const& pGltfModel, std::string const pSourceFolder)
    {
        std::vector<UUIDv4::UUID> textures(pGltfModel.textures.size());

        for (size_t i = 0; i < textures.size(); ++i)
        {
            tinygltf::Texture texture = pGltfModel.textures[i];
            std::shared_ptr<Texture> texturePointer = LoadTexture(pGltfModel, texture, pSourceFolder);
            textures[i] = texturePointer->GetAssetID();
        }

        return textures;
    }

    static bool isFileBinary(std::string const& pFilePath)
    {
        return (pFilePath.substr(pFilePath.find_last_of(".") + 1) == "bin");
    }

    /* Loads GLTF model from file. Returns tinygltf::Model object
    * .gltf files are effectively stored as json
    *
    * https://github.com/KhronosGroup/glTF-Tutorials/blob/main/gltfTutorial/gltfTutorial_004_ScenesNodes.md
    * GLTF format contains the following key items:
    *   - Scenes
    *   - Nodes
    *       - Transforms
    *   - Meshes
    *
    * Scenes are structured in a tree with a single root node and any amount of children per node.
    * Each node has its own transform that needs to be taken into consideration.
    */
    static tinygltf::Model LoadFromFile(std::string const& pFilePath)
    {
        tinygltf::Model gltfModel;
        tinygltf::TinyGLTF parser;
        std::string loaderErrors;
        std::string loaderWarnings;
        bool result = false;

        result = isFileBinary(pFilePath) ? parser.LoadBinaryFromFile(&gltfModel, &loaderErrors, &loaderWarnings, pFilePath)
            : parser.LoadASCIIFromFile(&gltfModel, &loaderErrors, &loaderWarnings, pFilePath);

        if (!loaderWarnings.empty())
        {
            printf("%s: warnings while loading glTF model:\n % s\n",
                __FUNCTION__, loaderWarnings.c_str());
        }
        if (!loaderErrors.empty())
        {
            printf("%s: errors while loading glTF model:\n % s\n",
                __FUNCTION__, loaderErrors.c_str());
        }
        if (!result)
        {
            printf("%s error: could not load file '%s'\n",
                __FUNCTION__, pFilePath.c_str());
        }
        if (gltfModel.scenes.size() != 1)
        {
            printf("%s error: GLTF model file must contain only one scene '%s'\n",
                __FUNCTION__, pFilePath.c_str());
            return {};
        }
        // return the processed model
        // printf("Loaded '%s'\n", pFilePath.c_str());
        return gltfModel;
    }

    Transform GetNodeLocalTransformMatrix(tinygltf::Node const& Node)
    {
        glm::vec3 translation{};
        if (!Node.translation.empty())
        {
            translation.x = static_cast<float>(Node.translation[0]);
            translation.y = static_cast<float>(Node.translation[1]);
            translation.z = static_cast<float>(Node.translation[2]);
        }

        glm::vec3 scale{ 1.0f };
        if (!Node.scale.empty())
        {
            scale.x = static_cast<float>(Node.scale[0]);
            scale.y = static_cast<float>(Node.scale[1]);
            scale.z = static_cast<float>(Node.scale[2]);
        }

        glm::quat rotation{};
        if (!Node.rotation.empty())
        {
            rotation.x = static_cast<float>(Node.rotation[0]);
            rotation.y = static_cast<float>(Node.rotation[1]);
            rotation.z = static_cast<float>(Node.rotation[2]);
            rotation.w = static_cast<float>(Node.rotation[3]);
        }

        Transform transform{ translation, rotation, scale };
        return transform;
    }

    static void LoadEntity(Entity* pEntity, tinygltf::Node& const node, tinygltf::Model& const pGltfModel, std::vector<UUIDv4::UUID>& pMaterials)
    {
        pEntity->SetDisplayName(node.name);

        Transform nodeTransform = GetNodeLocalTransformMatrix(node);
        pEntity->transform->setTransform(nodeTransform);

        int meshIndex = node.mesh;
        if (meshIndex >= 0)
        {
            Mesh* mesh = GLTFLoader::LoadMesh(pGltfModel, pGltfModel.meshes[meshIndex], pMaterials);
            MeshComponent* meshComponent = new MeshComponent(pEntity);
            meshComponent->setMesh(mesh);
            pEntity->addComponent(*meshComponent);
        }

        // Check if entity should have rigidbody
        std::regex self_regex("_RB\\b");
        if (std::regex_search(node.name, self_regex))
        {
            // Add rigidbody
            std::shared_ptr<AE86::RigidBody> rb = std::make_shared<AE86::RigidBody>();
            RigidBodyComponent* rigidComponent = new RigidBodyComponent(pEntity, rb);
            pEntity->addComponent(*rigidComponent);
        }
    }

    static void LoadChildEntities(Entity* pParentEntity, tinygltf::Model& const pGltfModel, std::vector<UUIDv4::UUID>& pMaterials, std::vector<int>& const pChildIndexes)
    {
        for (int childIndex : pChildIndexes)
        {
            tinygltf::Node node = pGltfModel.nodes[childIndex];
            Entity* childEntity = new Entity();
            pParentEntity->addChild(childEntity);
            childEntity->setParent(pParentEntity);

            LoadEntity(childEntity, node, pGltfModel, pMaterials);

            std::vector<int> childIndexes = node.children;
            LoadChildEntities(childEntity, pGltfModel, pMaterials, childIndexes);
        }
    }

    // Loads given model file as an entity
    static void LoadModelAsEntity(Entity* pEntity, std::string const pSourcePath, std::string const pModelFile)
    {
        
        tinygltf::Model gltfModel = LoadFromFile(DEFAULT_ASSET_FOLDER + pSourcePath + pModelFile);
        std::vector<UUIDv4::UUID> textures = LoadTextures(gltfModel, DEFAULT_ASSET_FOLDER + pSourcePath);
        std::vector<UUIDv4::UUID> materials = LoadMaterials(gltfModel, textures);

        // Traverse nodes and assign entities and components to the entity for each child
        tinygltf::Scene gltfScene = gltfModel.scenes[gltfModel.defaultScene];
        // Get Root nodes in model
        std::vector<int> rootIndexes = gltfScene.nodes;

        if (rootIndexes.size() > 1)
        {
            LoadChildEntities(pEntity, gltfModel, materials, rootIndexes);
            return;
        }

        tinygltf::Node rootNode = gltfModel.nodes[rootIndexes[0]];
        LoadEntity(pEntity, rootNode, gltfModel, materials);

        std::vector<int> childIndexes = rootNode.children;
        LoadChildEntities(pEntity, gltfModel, materials, childIndexes);
    }
}