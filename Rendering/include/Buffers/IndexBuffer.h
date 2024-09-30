#pragma once

#include <glad/glad.h>


class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	void Init(const void* data, unsigned int count);
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }

private:
	GLuint m_RendererID;
	unsigned int m_Count;
};

