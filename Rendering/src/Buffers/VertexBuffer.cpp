#include "Buffers/VertexBuffer.h"
#include "Utils/Logger.h"

VertexBuffer::VertexBuffer() : mRendererID(0)
{
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &mRendererID));
}

void VertexBuffer::Init(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &mRendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

