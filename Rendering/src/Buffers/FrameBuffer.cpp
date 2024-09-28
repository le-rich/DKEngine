#include "Buffers/FrameBuffer.h"
#include "Renderer.h"


FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
{
	Init(width, height);
}

FrameBuffer::~FrameBuffer()
{
	GLCall(glDeleteFramebuffers(1, &mBufferID));
}

void FrameBuffer::Resize(unsigned int width, unsigned int height)
{
	mBufferWidth = width;
	mBufferHeight = height;
	Unbind();
	Init(width, height);
}

void FrameBuffer::Bind() const
{
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mBufferID));
}

void FrameBuffer::Unbind() const
{
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
}

void FrameBuffer::Init(unsigned int width, unsigned int height)
{
	mBufferWidth = width;
	mBufferHeight = height;

	GLCall(glGenFramebuffers(1, &mBufferID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mBufferID));

	//Texture scaling property settings
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	//unbind to stop modifications
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
