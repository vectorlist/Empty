#include <Core/PCH.h>
#include <Graphics/FrameBuffer.h>

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
}

uint FrameBuffer::GetWidth() const
{
	return mWidth;
}

uint FrameBuffer::GetHeight() const
{
	return mHeight;
}

uint FrameBuffer::GetTextureCount() const
{
	return mTexture.size();
}

Texture* FrameBuffer::GetTexture(uint index)
{
	return mTexture[index].get();
}
