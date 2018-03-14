#pragma once

#include <Graphics/Buffer.h>
#include <core/type.h>
#include <vector>
#include <memory>

class UniformBuffer : public Buffer
{
public:
	UniformBuffer(){}
	virtual~UniformBuffer(){}

	virtual void Init(BufferCreateInfo& info) = 0;
	virtual void Update(const void* data) = 0;
	virtual void UpdateRanged(uint offset, uint size, const void* data) = 0;
	virtual void BindVS(uint index) = 0;
	virtual void BindPS(uint index) = 0;
	
private:
};
