#pragma once

#include <Core/Type.h>
#include <Graphics/Buffer.h>

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(){}
	virtual~IndexBuffer(){}

	virtual void Init(IndexBufferCreateInfo* info) = 0;
	virtual void Bind() = 0;
	virtual void UnBind() = 0;
	//specific Render (opntional)
	virtual void RenderIndexed(Topology topolgy, uint indexedCount) {};
};


class GLIndexBuffer : public IndexBuffer
{
public:
	GLIndexBuffer();
	virtual~GLIndexBuffer();
	void Init(IndexBufferCreateInfo* info) override;
	void Bind() override;
	void UnBind() override;
	void RenderIndexed(Topology topolgy, uint indexedCount) override;

	uint mIbo;
};

class DXIndexBuffer : public IndexBuffer
{
public:
	DXIndexBuffer();
	virtual~DXIndexBuffer();
	void Init(IndexBufferCreateInfo* info) override;
	void Bind() override;
	void UnBind() override;
	void RenderIndexed(Topology topolgy, uint indexedCount) override;

	ID3D11Buffer* mIbo;
};