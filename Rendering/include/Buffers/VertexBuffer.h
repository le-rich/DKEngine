#pragma once

#include <glad/glad.h>


class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
private:
	GLuint m_RendererID;
};