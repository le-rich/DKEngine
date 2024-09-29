#include "Buffers/UniformBuffer.h"
#include <glm.hpp>
#include "Renderer.h"


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

