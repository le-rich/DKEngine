#pragma once

#include <glad/glad.h>


class FrameBuffer
{
public:
	FrameBuffer(unsigned int width = 640, unsigned int height = 480);
	~FrameBuffer();

	void Resize(unsigned int width, unsigned int height);

	void Bind() const;
	void Unbind() const;

	void BindFrameTexture() const;
	void UnbindFrameTexture() const;

private:
	GLuint mBufferID, mColorBufferID, mRenderBufferID;
	unsigned int mBufferWidth;
	unsigned int mBufferHeight;

	void Init(unsigned int width, unsigned int height);
};

