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
	virtual void DrawIndexed(Topology topolgy, uint indexedCount) {};
	//@ usage : draw element by vertex base offset
	//opengl    : void DrawElementsBaseVertex(...base)
	//directx11 : voie DrawIndexed(...base)
	virtual void DrawIndexedBaseVertex(Topology topology, uint count, void* indices, uint baseVertex){}
};


class GLIndexBuffer : public IndexBuffer
{
public:
	GLIndexBuffer();
	virtual~GLIndexBuffer();
	void Init(IndexBufferCreateInfo* info) override;
	void Bind() override;
	void UnBind() override;
	void DrawIndexed(Topology topolgy, uint indexedCount) override;
	void DrawIndexedBaseVertex(Topology topology, uint count, void* indices, uint baseVertex) override;
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
	void DrawIndexed(Topology topolgy, uint indexedCount) override;
	void DrawIndexedBaseVertex(Topology topology, uint count, void* indices, uint baseVertex) override;

	ID3D11Buffer* mIbo;
};