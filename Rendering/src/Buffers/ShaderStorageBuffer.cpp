#include "Buffers/ShaderStorageBuffer.h"

ShaderStorageBuffer::ShaderStorageBuffer(GLenum pUsage)
{
    glGenBuffers(1, &mBufferID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mBufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, static_cast<GLenum>(pUsage));
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mBufferID);
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
    glDeleteBuffers(1, &mBufferID);
}

void ShaderStorageBuffer::Bind(GLuint pBindingPoint)
{
    mBindingPoint = pBindingPoint;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, pBindingPoint, mBufferID);
}

void ShaderStorageBuffer::Unbind()
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, mBindingPoint, 0);
}
