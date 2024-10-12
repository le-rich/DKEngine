#pragma once

#include <glad/glad.h>
#include "VertexBuffer.h"
#include "Data/VertexBufferLayout.h"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;
private:
    GLuint mRendererID;
};
