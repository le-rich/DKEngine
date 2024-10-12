#pragma once

#include <tiny_gltf.h>

#include "Resources/Mesh.h"

namespace GLTFLoader
{

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


    static bool GetAttributeVector(tinygltf::Model const& pGltfModel, uint32_t const accessorNum, std::vector<unsigned char>& OutBufferData, int numberOfElements)
    {
        const tinygltf::Accessor& accessor = pGltfModel.accessors[accessorNum];
        const tinygltf::BufferView& bufferView = pGltfModel.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = pGltfModel.buffers[bufferView.buffer];

        int typeCount = GetNumberOfComponentsInType(accessor.type);

        size_t componentType = accessor.componentType;
        size_t componentSize = GetComponentsSize(componentType);

        for (int element = 0; element < numberOfElements; ++element)
        {
            auto start = buffer.data.begin() + bufferView.byteOffset + (element * bufferView.byteStride);
            auto end = buffer.data.begin() + bufferView.byteOffset + (element * bufferView.byteStride) + (componentSize * typeCount);

            std::copy(start, end, OutBufferData.begin() + (componentSize * typeCount * element));
        }
        return true;
    }


    static Primitive ProcessPrimitive(tinygltf::Model pGltfModel, tinygltf::Primitive pPrimitive)
    {
        std::vector<Vertex> vertices;
        int numOfElements = GetMaxElementCount(pGltfModel, pPrimitive);
        vertices.reserve(sizeof(Vertex) * numOfElements);
        for (const auto& attribute : pPrimitive.attributes)
        {
            const std::string attribType = attribute.first;
            const int accessorNum = attribute.second;

            std::vector<unsigned char> bufferData;
            if (!GetAttributeVector(pGltfModel, accessorNum, bufferData, vertices.size()))
            {
                std::printf("Failed to get attribute data");
                continue;
            }

            std::vector<float> floatVector(bufferData.size());
            std::memcpy(floatVector.data(), bufferData.data(), bufferData.size());

            // TODO: Modify to support multi texturing
            switch (GetVertexAttributeFromString(attribType))
            {
            case VertexAttributeKeys::POSITION:
                for (int index = 0; index < vertices.size(); index += 3)
                {
                    vertices[index].mPosition = glm::vec3(floatVector[index], floatVector[index + 1], floatVector[index + 2]);
                }
                break;
            case VertexAttributeKeys::NORMAL:
                for (int index = 0; index < vertices.size(); index += 3)
                {
                    vertices[index].mNormal = glm::vec3(floatVector[index], floatVector[index + 1], floatVector[index + 2]);
                }
                break;
            case VertexAttributeKeys::TEXCOORD_0:
                for (int index = 0; index < vertices.size(); index += 2)
                {
                    vertices[index].mUVCoord = glm::vec2(floatVector[index], floatVector[index + 2]);
                }
                break;
            default:
                printf("Unsupported attribute data type!");
                continue;

            }
        }


        std::vector<unsigned char> bufferData;
        if (!GetAttributeVector(pGltfModel, pPrimitive.indices, bufferData, pGltfModel.accessors[pPrimitive.indices].count))
        {
            std::printf("Failed to get attribute data");
        }
        const auto bufferDataSizeBytes = sizeof(unsigned char) * bufferData.size();

        std::vector<uint32_t> indices;
        indices.reserve(bufferDataSizeBytes);
        std::memcpy(&indices, bufferData.data(), bufferDataSizeBytes);

        Primitive primitive(vertices, indices);
        return primitive;
    }

    static Mesh LoadMesh(tinygltf::Model pGltfModel, tinygltf::Mesh pMesh)
    {
        Mesh mesh;
        for (tinygltf::Primitive const& primitive : pMesh.primitives)
            mesh.AddPrimitive(ProcessPrimitive(pGltfModel, primitive));
        return mesh;
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
        return gltfModel;
    }
}