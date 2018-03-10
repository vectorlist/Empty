#pragma once

#include <graphics/dx11/dxconfig.h>
#include <graphics/uniformbuffer.h>
#include <graphics/context.h>

class DXConstantBuffer : public UniformBuffer
{
public:
	DXConstantBuffer();
	virtual~DXConstantBuffer();

	void Init(BufferCreateInfo& info) override;
	void Update(const void* data) override;
	void UpdateRanged(uint offset, uint size, const void* data) override;
	void BindVS(uint index)override;
	void BindPS(uint index)override;

	ID3D11Buffer* mBuffer;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;
};


//D3D11_MAPPED_SUBRESOURCE mappedResource;
//ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
//float2 vertices[] =
//{
//	float2(-1.0f, -1.0f),
//	float2(0.0f, 1.0f),
//	float2(1.0f, -1.0f),
//};
////	Disable GPU access to the vertex buffer data.
//auto m_d3dContext = m_deviceResources->GetD3DDeviceContext();
//m_d3dContext->Map(vertexBuffer2.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
////	Update the vertex buffer here.
//memcpy(mappedResource.pData, vertices, sizeof(vertices));
////	Reenable GPU access to the vertex buffer data.
//m_d3dContext->Unmap(vertexBuffer2.Get(), 0);

//D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
//vertexBufferDesc.ByteWidth = sizeof(float2)* ARRAYSIZE(triangleVertices);
//vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//vertexBufferDesc.MiscFlags = 0;
//vertexBufferDesc.StructureByteStride = 0;
//
//D3D11_SUBRESOURCE_DATA vertexBufferData;
//vertexBufferData.pSysMem = triangleVertices;
//vertexBufferData.SysMemPitch = 0;
//vertexBufferData.SysMemSlicePitch = 0;
//
//DX::ThrowIfFailed(
//	m_d3dDevice->CreateBuffer(
//		&vertexBufferDesc,
//		&vertexBufferData,
//		&vertexBuffer2
//	)
//);