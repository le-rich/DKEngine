#include "Buffers/UniformBuffer.h"

#include <glm.hpp>
#include "Utils/Logger.h"
#include <gtc/type_ptr.hpp>


UniformBuffer::UniformBuffer()
{
	const size_t bufferSize = 3 * sizeof(glm::mat4) + sizeof(glm::vec3);

    GLCall(glGenBuffers(1, &mBufferID));
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, mBufferID));
    GLCall(glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW)); // Changed to DYNAMIC_DRAW
    GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT, mBufferID)); // Use Base instead of Range
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));	
}

UniformBuffer::~UniformBuffer()
{
	GLCall(glDeleteBuffers(1, &mBufferID));
}

void UniformBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, mBufferID));
}

void UniformBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UniformBuffer::updateMatrices(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) {
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, mBufferID));
    
    // Update all matrices in one go
    size_t offset = 0;
    GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(model)));
    offset += sizeof(glm::mat4);
    GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(view)));
    offset += sizeof(glm::mat4);
    GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(projection)));
    offset += sizeof(glm::mat4);
    GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(cameraPos)));
    
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UniformBuffer::SetCameraMatrices(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos)
{
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, mBufferID));

    // Update all Camera matrices in one go
    size_t offset = sizeof(glm::mat4);
    GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(view)));
    offset += sizeof(glm::mat4);
    GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(projection)));
    offset += sizeof(glm::mat4);
    GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(cameraPos)));

    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}
