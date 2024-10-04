#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm.hpp>

#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexArray.h"
#include "Data/VertexBufferLayout.h"
#include "Data/Vertex.h"


class Primitive
{
public:
    Primitive(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
    ~Primitive();

    void Draw();
    inline VertexBuffer& GetVertexBuffer() { return mVertexBuffer; }
    inline IndexBuffer& GetIndexBuffer() { return mIndexBuffer; }

private:
    VertexBuffer mVertexBuffer;
    IndexBuffer mIndexBuffer;
    VertexArray mVertexArray;
    VertexBufferLayout mVertexBufferLayout;
    std::vector<Vertex> mLoadedVertices;
    std::vector<GLuint> mLoadedIndices;
    GLuint mMaterialID;
};

// Base struct for shapes
struct Shape
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
};

// Derived struct for Triangle
struct Triangle : public Shape
{
    Triangle()
    {
        vertices.resize(3);
        vertices.resize(3);
        vertices =
        {
            Vertex(glm::vec3(-0.5f, -0.5f, 0.0f)),  // 0
            Vertex(glm::vec3( 0.0f,  0.5f, 0.0f)),  // 1
            Vertex(glm::vec3( 0.5f, -0.5f, 0.0f))   // 2
        };
        indices = { 0, 1, 2 };
    }
};

// Derived struct for Square
struct Square : public Shape
{
    Square()
    {
        vertices.resize(4);
        vertices.resize(6);
        vertices =
        {
            Vertex(glm::vec3(-0.5f, -0.5f, 0.0f)), // 0
            Vertex(glm::vec3( 0.5f, -0.5f, 0.0f)), // 1
            Vertex(glm::vec3( 0.5f,  0.5f, 0.0f)), // 2
            Vertex(glm::vec3(-0.5f,  0.5f, 0.0f))  // 3
        };
        indices =
        {
            0, 1, 2,
            2, 3, 0
        };
    }
};

// Derived struct for Cube
struct Cube : public Shape
{
    Cube()
    {
        vertices.resize(36);
        vertices =
        {
            // front
            Vertex(glm::vec3(-0.5f, -0.5f, 0.5f)),
            Vertex(glm::vec3(0.5f, 0.5f, 0.5f)),
            Vertex(glm::vec3(-0.5f, 0.5f, 0.5f)),
            Vertex(glm::vec3(-0.5f, -0.5f, 0.5f)),
            Vertex(glm::vec3(0.5f, -0.5f, 0.5f)),
            Vertex(glm::vec3(0.5f, 0.5f, 0.5f)),

            // back
            Vertex(glm::vec3(-0.5f, -0.5f, -0.5f)),
            Vertex(glm::vec3(-0.5f, 0.5f, -0.5f)),
            Vertex(glm::vec3(0.5f, 0.5f, -0.5f)),
            Vertex(glm::vec3(-0.5f, -0.5f, -0.5f)),
            Vertex(glm::vec3(0.5f, 0.5f, -0.5f)),
            Vertex(glm::vec3(0.5f, -0.5f, -0.5f)),

            // left
            Vertex(glm::vec3(-0.5f, -0.5f, 0.5f)),
            Vertex(glm::vec3(-0.5f, 0.5f, 0.5f)),
            Vertex(glm::vec3(-0.5f, 0.5f, -0.5f)),
            Vertex(glm::vec3(-0.5f, -0.5f, 0.5f)),
            Vertex(glm::vec3(-0.5f, 0.5f, -0.5f)),
            Vertex(glm::vec3(-0.5f, -0.5f, -0.5f)),

            // right
            Vertex(glm::vec3(0.5f, -0.5f, 0.5f)),
            Vertex(glm::vec3(0.5f, 0.5f, -0.5f)),
            Vertex(glm::vec3(0.5f, 0.5f, 0.5f)),
            Vertex(glm::vec3(0.5f, -0.5f, 0.5f)),
            Vertex(glm::vec3(0.5f, -0.5f, -0.5f)),
            Vertex(glm::vec3(0.5f, 0.5f, -0.5f)),

            // top
            Vertex(glm::vec3(0.5f, 0.5f, 0.5f)),
            Vertex(glm::vec3(-0.5f, 0.5f, -0.5f)),
            Vertex(glm::vec3(-0.5f, 0.5f, 0.5f)),
            Vertex(glm::vec3(0.5f, 0.5f, 0.5f)),
            Vertex(glm::vec3(0.5f, 0.5f, -0.5f)),
            Vertex(glm::vec3(-0.5f, 0.5f, -0.5f)),

            // bottom
            Vertex(glm::vec3(0.5f, -0.5f, 0.5f)),
            Vertex(glm::vec3(-0.5f, -0.5f, 0.5f)),
            Vertex(glm::vec3(-0.5f, -0.5f, -0.5f)),
            Vertex(glm::vec3(0.5f, -0.5f, 0.5f)),
            Vertex(glm::vec3(-0.5f, -0.5f, -0.5f)),
            Vertex(glm::vec3(0.5f, -0.5f, -0.5f))
        };
        indices =
        {
            // Front
            0, 1, 2,
            2, 3, 0,
            // Back 
            4, 5, 6,
            6, 7, 4,
            // Left 
            4, 0, 3,
            3, 7, 4,
            // Right
            1, 5, 6,
            6, 2, 1,
            // Top
            3, 2, 6,
            6, 7, 3,
            // Bottom
            4, 5, 1,
            1, 0, 4
        };
    }
};

