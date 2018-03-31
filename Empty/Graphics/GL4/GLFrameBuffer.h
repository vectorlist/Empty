#pragma once

#include <Graphics/FrameBuffer.h>

class GLFrameBuffer : public FrameBuffer
{
public:
	GLFrameBuffer();
	virtual ~GLFrameBuffer();

	void Init(FrameBufferCreateInfo* info) override;
	void Clear(float r, float g, float b, float a) override;
	void Bind() override;
	void UnBind() override;

	//Test
	uint GetFrameBuffer() const { return mFrameBuffer; }
	bool BlitDepthStencilBuffer(uint x, uint y, uint w, uint h);
private:
	uint mFrameBuffer;
	uint mDepthBuffer;
};