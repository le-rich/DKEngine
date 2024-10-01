#pragma once

#include <glad/glad.h>


class UniformBuffer
{
public:
	UniformBuffer();
	~UniformBuffer();

	void Bind() const;
	void Unbind() const;

private:
	GLuint mBufferID;

};
