#pragma once

#include <Graphics/VertexBuffer.h>

class DXVertexBuffer : public VertexBuffer
{
public:
	DXVertexBuffer();
	virtual~DXVertexBuffer();

	void Init(VertexBufferCreateInfo* info) override;
	
	void SubData(uint offset, uint size, void* data) override;
	void Bind() override;
	void UnBind() override;

	void CreateInputLayout(VertexBufferCreateInfo& info);
protected:
	ID3D11Buffer*			mVbo;
	ID3D11InputLayout*		mInputLayout;
	uint					mStride;
	uint					mOffset;
};