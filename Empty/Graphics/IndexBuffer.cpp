#include <Core/PCH.h>
#include <Graphics/IndexBuffer.h>

#include <Graphics/TypeTransform.h>
#include <Graphics/GL4/GLConfig.h>
#include <Graphics/DX11/DXContext.h>

//=========================== GL Index Buffer =================================

GLIndexBuffer::GLIndexBuffer()
	: mIbo(0)
{
}

GLIndexBuffer::~GLIndexBuffer()
{
	if (mIbo) glDeleteBuffers(1, &mIbo);
}

void GLIndexBuffer::Init(IndexBufferCreateInfo* info)
{
	glGenBuffers(1, &mIbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, info->IndicesSize, info->pIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLIndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
}

void GLIndexBuffer::UnBind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}

void GLIndexBuffer::DrawIndexed(Topology topolgy, uint indexedCount)
{
	glDrawElements(GLTransform::GetTopology(topolgy), indexedCount, GL_UNSIGNED_INT, NULL);
}

void GLIndexBuffer::DrawIndexedBaseVertex(Topology topology, uint count, void* indices, uint baseVertex)
{
	glDrawElementsBaseVertex(GLTransform::GetTopology(topology), count, GL_UNSIGNED_INT, indices, baseVertex);
}


//=========================== DX Index Buffer =================================

DXIndexBuffer::DXIndexBuffer()
	: mIbo(nullptr)
{
}

DXIndexBuffer::~DXIndexBuffer()
{
	SAFE_RELEASE(mIbo);
}

void DXIndexBuffer::Init(IndexBufferCreateInfo* info)
{
	D3D11_BUFFER_DESC bufferInfo{};
	bufferInfo.Usage = D3D11_USAGE_DEFAULT;
	bufferInfo.ByteWidth = info->IndicesSize;
	bufferInfo.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferInfo.CPUAccessFlags = 0;
	bufferInfo.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA bufferData{};
	bufferData.pSysMem = info->pIndices;

	LOG_HR << G_DXDevice->CreateBuffer(&bufferInfo, &bufferData, &mIbo);
}

void DXIndexBuffer::Bind()
{
	G_DXContext->IASetIndexBuffer(mIbo, DXGI_FORMAT_R32_UINT, 0);
}

void DXIndexBuffer::UnBind()
{
	//Do Nothing
}

void DXIndexBuffer::DrawIndexed(Topology topolgy, uint indexedCount)
{
	G_DXContext->IASetPrimitiveTopology(DXTransform::GetTopology(topolgy));
	G_DXContext->DrawIndexed(indexedCount, 0, 0);
}

void DXIndexBuffer::DrawIndexedBaseVertex(Topology topology, uint count, void * indices, uint baseVertex)
{
	G_DXContext->IASetPrimitiveTopology(DXTransform::GetTopology(topology));
	G_DXContext->DrawIndexed(count, 0, baseVertex);
}
