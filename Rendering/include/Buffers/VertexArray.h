#pragma once

#include <glad/glad.h>
#include "VertexBuffer.h"

class VertexBufferLayout;


class VertexArray
{
private:
	GLuint mRendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};
