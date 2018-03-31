#pragma once

#include <Graphics/Buffer.h>
#include <Core/Type.h>

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(){}
	virtual~VertexBuffer(){}

	virtual void Init(VertexBufferCreateInfo* info) = 0;
	virtual void Map(void* data){}

	virtual void SubData(uint offset, uint size, void* data) = 0;
	virtual void Bind() = 0;
	virtual void UnBind() = 0;
	virtual void DrawArray(Topology topology, uint offset, uint count){}
};

