#pragma once

#include <Graphics/VertexBuffer.h>

class GLVertexBuffer : public VertexBuffer
{
public:
	GLVertexBuffer();
	virtual~GLVertexBuffer();

	void Init(VertexBufferCreateInfo* info) override;
	
	void Bind() override;
	void UnBind() override;

	void SubData(uint offset, uint size, void* data) override;
	
	uint vao;
	uint vbo;
	
};