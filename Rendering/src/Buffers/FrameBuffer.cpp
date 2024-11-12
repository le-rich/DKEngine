#include "Buffers/FrameBuffer.h"
#include "Utils/Logger.h"

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

    // Resize Color attachment texture
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mBufferID));
    glBindTexture(GL_TEXTURE_2D, mColorBufferID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mBufferWidth, mBufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBufferID, 0);

    // Resize Renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mBufferWidth, mBufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mBufferID));
    glViewport(0, 0, mBufferWidth, mBufferHeight);
}

void FrameBuffer::Unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::BindFrameTexture() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, mColorBufferID));
}

void FrameBuffer::UnbindFrameTexture() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void FrameBuffer::Init(unsigned int width, unsigned int height)
{
    mBufferWidth = width;
    mBufferHeight = height;

    GLCall(glGenFramebuffers(1, &mBufferID));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mBufferID));

    // create a color attachment texture
    glGenTextures(1, &mColorBufferID);
    glBindTexture(GL_TEXTURE_2D, mColorBufferID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mBufferWidth, mBufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBufferID, 0);

    // create a renderbuffer object
    glGenRenderbuffers(1, &mRenderBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mBufferWidth, mBufferHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferID);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
