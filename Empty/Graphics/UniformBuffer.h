#pragma once

#include <core/type.h>
#include <vector>
#include <memory>

class Buffer 
{
public:
	Buffer(){}
	virtual~Buffer(){}

};

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


//TODO Vertex Buffer and Instancing Buffer
class VertexBuffer : public Buffer
{
public:
	virtual void Init(BufferCreateInfo& info) = 0;
};

struct VertexInstancingBuffer : public Buffer
{
public:
	virtual void Init(BufferCreateInfo& info) = 0;
};
