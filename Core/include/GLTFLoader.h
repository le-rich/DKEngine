#pragma once


#include <tiny_gltf.cc>

#include "Managers/AssetManager.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "Resources/Texture.h"

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

            //std::copy(start, end, std::back_inserter(OutBufferData));
            OutBufferData.insert(OutBufferData.end(), start, end);
        }
        return true;
    }

    /*
    * Returns primitive object
    */
    static Primitive ProcessPrimitive(tinygltf::Model pGltfModel, tinygltf::Primitive pPrimitive)
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
                std::printf("Failed to get attribute data");
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

        // TODO: Add material index
        Primitive primitive(vertices, indices);
        return primitive;
    }

    /*
    * Returns mesh
    */
    static Mesh LoadMesh(tinygltf::Model pGltfModel, tinygltf::Mesh pMesh)
    {
        Mesh mesh;
        for (tinygltf::Primitive const& primitive : pMesh.primitives)
            mesh.AddPrimitive(ProcessPrimitive(pGltfModel, primitive));
        return mesh;
    }

    // TODO
    static Material LoadMaterial()
    {
        return Material();
    }


    // Load Textures into asset manager and return list of id's for this set of textures
    static std::vector<std::shared_ptr<Texture>> LoadTextures(tinygltf::Model const& pGltfModel, std::string const pSourceFolder)
    {
        std::vector<std::shared_ptr<Texture>> textures(pGltfModel.textures.size());

        for (size_t i = 0; i < textures.size(); ++i)
        {
            // TODO: Get and set image sampling properties and pass to Texture object
            tinygltf::Texture texture = pGltfModel.textures[i];
            int imageIndex = texture.source;
            tinygltf::Image image = pGltfModel.images[imageIndex];
            std::shared_ptr<Texture> texturePointer = std::make_shared<Texture>(pSourceFolder + image.uri);
            AssetManager::GetInstance().AddTexture(texturePointer);
            textures[i] = texturePointer;
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
        printf("Loaded '%s'\n", pFilePath.c_str());
        return gltfModel;
    }
}