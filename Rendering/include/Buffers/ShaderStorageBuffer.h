#pragma once

#include <glad/glad.h>

class ShaderStorageBuffer
{
public:
    // pUsage is a hint to the GL implementation as to how a buffer object's data store will be accessed. 
    // Reference glBufferData for more information.
    ShaderStorageBuffer(GLenum pUsage = GL_STATIC_DRAW);
    ~ShaderStorageBuffer();

    // Bind shader storage buffer to specified point
    // Default value: 0
    void Bind(GLuint pBindingPoint = 0);
    void Unbind();

    // Write given data to the buffer.
    template<typename T>
    void SendBlocks(T* pData, size_t pSize)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, mBufferID);
        glBufferData(GL_SHADER_STORAGE_BUFFER, pSize, pData, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

private:
    GLuint mBufferID; 
    GLuint mBindingPoint = 0;
};

