#pragma once

#include <graphics/uniformbuffer.h>

class GLUnifomBuffer : public UniformBuffer
{
public:
	GLUnifomBuffer();
	virtual~GLUnifomBuffer();

	void Init(BufferCreateInfo* info) override;
	void Update(const void* data) override;
	void UpdateRanged(uint offset, uint size, const void* data) override;
	void BindVS(uint index) override;
	void BindPS(uint index) override;
	
	uint ubo;
	uint mSize;
};


