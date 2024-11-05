#pragma once

#include <glad/glad.h>
#include <glm.hpp>
#include <vector>
#include <memory>

#include "Utils/IDUtils.h"
#include "Buffers/VertexArray.h"
#include "Buffers/IndexBuffer.h"
#include "Data/Vertex.h"


class Primitive
{
public:
    Primitive(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const UUIDv4::UUID& pMaterialID = 0);
    ~Primitive();

    void Draw();
    void DrawWithOwnMaterial();
    inline void SetMaterial(UUIDv4::UUID& pMaterialID) { mMaterialID = pMaterialID; }
    inline const UUIDv4::UUID GetMaterial() { return mMaterialID; }

private:
    std::shared_ptr<VertexBuffer> mVertexBuffer;
    std::shared_ptr<IndexBuffer> mIndexBuffer;
    std::shared_ptr<VertexArray> mVertexArray;
    VertexBufferLayout mVertexBufferLayout;
    std::vector<Vertex> mLoadedVertices;
    std::vector<uint32_t> mLoadedIndices;
    UUIDv4::UUID mMaterialID;
};

// Base struct for shapes
struct Shape
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

// Derived struct for Cube
struct Cube : public Shape
{
    Cube()
    {
        vertices.resize(8);
        // Facing from front
        vertices[0].mPosition = glm::vec3(-0.5f, -0.5f, 0.5f);  // Front Bot Left
        vertices[1].mPosition = glm::vec3(0.5f, -0.5f, 0.5f);   // Front Bot Right
        vertices[2].mPosition = glm::vec3(0.5f, 0.5f, 0.5f);    // Front Top Right
        vertices[3].mPosition = glm::vec3(-0.5f, 0.5f, 0.5f);   // Front Top Left
        vertices[4].mPosition = glm::vec3(-0.5f, -0.5f, -0.5f); // Back Bot Left
        vertices[5].mPosition = glm::vec3(0.5f, -0.5f, -0.5f);  // Back Bot Right
        vertices[6].mPosition = glm::vec3(0.5f, 0.5f, -0.5f);   // Back Top Right
        vertices[7].mPosition = glm::vec3(-0.5f, 0.5f, -0.5f);  // Back Top Left

        vertices[0].mNormal = glm::vec3(1.0f, 0.5f, 0.5f);
        vertices[1].mNormal = glm::vec3(1.0f, 0.5f, 0.5f);
        vertices[2].mNormal = glm::vec3(1.0f, 0.5f, 0.5f);
        vertices[3].mNormal = glm::vec3(1.0f, 0.5f, 0.5f);
        vertices[4].mNormal = glm::vec3(0.5f, 1.0f, 0.5f);
        vertices[5].mNormal = glm::vec3(0.5f, 1.0f, 0.5f);
        vertices[6].mNormal = glm::vec3(0.5f, 1.0f, 0.5f);
        vertices[7].mNormal = glm::vec3(0.5f, 1.0f, 0.5f);

        vertices[0].mUVCoord = glm::vec2(0.0, 0.0);
        vertices[1].mUVCoord = glm::vec2(1.0, 1.0);
        vertices[2].mUVCoord = glm::vec2(0.0, 1.0);
        vertices[3].mUVCoord = glm::vec2(1.0, 0.0);
        vertices[4].mUVCoord = glm::vec2(1.0, 0.0);
        vertices[5].mUVCoord = glm::vec2(0.0, 1.0);
        vertices[6].mUVCoord = glm::vec2(1.0, 1.0);
        vertices[7].mUVCoord = glm::vec2(0.0, 0.0);

        indices =
        {
            // Front
            0, 1, 2,
            0, 2, 3,
            // Back 
            4, 6, 5,
            4, 7, 6,
            // Left 
            0, 4, 7,
            0, 7, 3,
            // Right
            1, 5, 6,
            1, 6, 2,
            // Top
            3, 2, 6,
            3, 6, 7,
            // Bottom
            0, 1, 5,
            0, 5, 4
        };
    }
};

