#pragma once

//TYPE
// FRAMEBUFFER      SINGLE
// FRAMEBUFFER      MTR
// FRAMEBUFFER		MTR + DEPTH STENCIL

class FrameBuffer
{
public:
	FrameBuffer();
	virtual~FrameBuffer();

	virtual void Init(FrameBufferCreateInfo* info) = 0;
	virtual void Clear(float r, float g, float b, float a) = 0;	//TODO OPTIONAL
	virtual void Bind() = 0;
	virtual void UnBind() = 0;

	uint		GetWidth() const;
	uint		GetHeight() const;
	uint		GetTextureCount() const;
	Texture*	GetTexture(uint index);
	
	virtual bool BlitDepthStencilBuffer(uint x, uint y, uint w, uint h) = 0;
protected:
	uint mWidth;
	uint mHeight;
	FrameBufferType mType;

	std::vector<std::shared_ptr<Texture>> mTexture;
};