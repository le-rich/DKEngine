#include "Buffers/UniformBuffer.h"

#include <glm.hpp>
#include "Utils/Logger.h"
#include <gtc/type_ptr.hpp>


UniformBuffer::UniformBuffer()
{
	// size of data in buffer object
	size_t size = sizeof(glm::mat4) +
		sizeof(glm::mat4) +
		sizeof(glm::mat4) +
		sizeof(glm::vec3);

	// Create & bind buffer
	GLCall(glGenBuffers(1, &mBufferID));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, mBufferID));
	// Allocate initial memory for buffer & indicate range of indexed buffer 
	GLCall(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW));
	GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, 0, mBufferID, 0, size));
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

void UniformBuffer::UploadUboData(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 viewPosition)
{
	size_t offset = sizeof(glm::mat4);
	glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
	offset += sizeof(glm::mat4);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
	offset += sizeof(glm::mat4);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(viewPosition));
	//glBindBufferRange(GL_UNIFORM_BUFFER, 0, mUboBuffer, 0, 2 * sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//SetSubData(viewMatrix, offset);
	//SetSubData(projectionMatrix, offset);
	//SetSubData(viewPosition, offset);

}