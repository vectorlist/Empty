#pragma once

#include <Graphics/Buffer.h>
#include <Core/Type.h>
#include <Graphics/Buffer.h>

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(){}
	virtual~VertexBuffer(){}

	virtual void Init(BufferCreateInfo& info) = 0;

	virtual void Map(void* data){}
	virtual void SetInputLayout(std::vector<InputLayout>& layout) {};

	virtual void SubData(uint offset, uint size, void* data) = 0;
	virtual void Bind() = 0;
};

class GLVertexBuffer : public VertexBuffer
{
public:
	GLVertexBuffer();
	virtual~GLVertexBuffer();

	void Init(BufferCreateInfo& info) override;
	void SetInputLayout(std::vector<InputLayout>& layout) override;
	void Bind() override;

	void SubData(uint offset, uint size, void* data) override;
	
	uint vao;
	uint vbo;
	
};