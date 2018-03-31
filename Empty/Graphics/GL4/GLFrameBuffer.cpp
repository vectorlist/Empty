#include <Core/PCH.h>
#include <GRaphics/GL4/GLFrameBuffer.h>
#include <Graphics/GL4/GLTexture.h>
#include <Graphics/GL4/GLConfig.h>
#include <Graphics/TypeTransform.h>

GLFrameBuffer::GLFrameBuffer()
	: mFrameBuffer(0), mDepthBuffer(0)
{
}

GLFrameBuffer::~GLFrameBuffer()
{
}

void GLFrameBuffer::Init(FrameBufferCreateInfo* info)
{
	this->mWidth = info->Width;
	this->mHeight = info->Height;
	this->mType = info->Type;

	glGenFramebuffers(1, &mFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

	std::vector<uint> attachments;

	for (uint i = 0; i < info->BufferCount; ++i)
	{
		std::shared_ptr<GLTexture> texture = std::make_shared<GLTexture>();

		texture->width = mWidth;
		texture->height = mHeight;

		glGenTextures(1, &texture->mTexture);
		glBindTexture(GL_TEXTURE_2D, texture->mTexture);

		glTexImage2D(GL_TEXTURE_2D, 0,
			GLTransform::GetInternalFormat(info->BufferFormat),
			mWidth, mHeight, 0,
			GLTransform::GetFormat(info->BufferFormat), GL_FLOAT, nullptr);

		glTextureParameteri(texture->mTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(texture->mTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//let texture can wirte as frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->mTexture, 0);

		attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
		mTexture.emplace_back(texture);
	}

	//let them output as Color Buffer in Frament Shader
	glDrawBuffers(attachments.size(), attachments.data());

	if ((mType & FRAMEBUFFER_MTR_DEPTHSTENCIL) != 0) {
		glGenRenderbuffers(1, &mDepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);
	}
	//Create Depth (Render Target)

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		ASSERT_MSG(0, "failed to bind render buffer");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
* Clear Color Bit And Deapth Bit
*/
void GLFrameBuffer::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLFrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
}

void GLFrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}

bool GLFrameBuffer::BlitDepthStencilBuffer(uint x, uint y, uint w, uint h)
{
	if (mDepthBuffer == 0) return false;
	//TODO : Check Has Depth Stencil
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mFrameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, NULL);
	
	glBlitFramebuffer(x, y, w, h, x, y, w, h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	return true;
}
