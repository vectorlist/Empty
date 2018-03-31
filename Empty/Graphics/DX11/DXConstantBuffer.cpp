#include <Core/PCH.h>
#include <graphics/dx11/dxconstantbuffer.h>

DXConstantBuffer::DXConstantBuffer()
	: mBuffer(nullptr), mDevice(nullptr), mContext(nullptr)
{
	mDevice = G_DXDevice;
	mDevice->GetImmediateContext(&mContext);
}

DXConstantBuffer::~DXConstantBuffer()
{
	SAFE_RELEASE(mBuffer);
}

void DXConstantBuffer::Init(BufferCreateInfo* info)
{
	//just default for now
	D3D11_BUFFER_DESC bufferInfo{};
	bufferInfo.Usage = D3D11_USAGE_DEFAULT;
	bufferInfo.ByteWidth = info->size;
	bufferInfo.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferInfo.CPUAccessFlags = 0;

	//just define size
	LOG_HR << mDevice->CreateBuffer(&bufferInfo, nullptr, &mBuffer);
}

void DXConstantBuffer::Update(const void* data)
{
	mContext->UpdateSubresource(mBuffer, 0, 0, data, 0, 0);
}

void DXConstantBuffer::UpdateRanged(uint offset, uint size, const void* data)
{
	//const D3D11_BOX dstBox = { offset, 0U, 0U, size, 1U, 1U };
	mContext->UpdateSubresource(mBuffer, 0, 0, data, 0, 0);
	
}

inline void DXConstantBuffer::BindVS(uint index)
{
	mContext->VSSetConstantBuffers(index, 1, &mBuffer);
}

inline void DXConstantBuffer::BindPS(uint index)
{
	mContext->PSSetConstantBuffers(index, 1, &mBuffer);
}

//void DXConstantBuffer::Init(BufferCreateInfo& info)
//{
//	//just default for now
//	D3D11_BUFFER_DESC bufferInfo{};
//	bufferInfo.Usage = D3D11_USAGE_DYNAMIC;
//	bufferInfo.ByteWidth = info.size;
//	bufferInfo.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	bufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//
//	//just define size
//	LOG_HR << mDevice->CreateBuffer(&bufferInfo, nullptr, &mBuffer);
//}
//
//void DXConstantBuffer::UpdateRanged(uint offset, uint size, const void* data)
//{
//	//mContext->UpdateSubresource(mBuffer, 0, 0, data, 0, 0);
//	D3D11_MAPPED_SUBRESOURCE mapped{};
//
//	
//	LOG_HR << mContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
//
//	//void* dataPtr = static_cast<char*>(mapped.pData) + offset;
//	//dataPtr = (void*)data;
//
//	uint psize = sizeof(mapped.pData);
//
//	void* dataPtr = mapped.pData;
//	dataPtr = (void*)data;
//
//	mContext->Unmap(mBuffer, 0);
//}
//
//inline void DXConstantBuffer::BindVS(uint index)
//{
//	mContext->VSSetConstantBuffers(index, 1, &mBuffer);
//}
//
//inline void DXConstantBuffer::BindPS(uint index)
//{
//	mContext->PSSetConstantBuffers(index, 1, &mBuffer);
//}