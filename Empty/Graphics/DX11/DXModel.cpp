#include <PCH.h>
#include "DXModel.h"
#include <graphics/dx11/dxcontext.h>

DXModel::DXModel()
	: mVertexBuffer(nullptr), mIndexBuffer(nullptr)
{

}

DXModel::~DXModel()
{
	SAFE_RELEASE(mVertexBuffer);
	SAFE_RELEASE(mIndexBuffer);
}

void DXModel::CreateBuffer(std::vector<Vertex>& vertice, std::vector<uint>& indice)
{
	D3D11_BUFFER_DESC vtxInfo{};
	vtxInfo.Usage = D3D11_USAGE_DEFAULT;
	vtxInfo.ByteWidth = sizeof(Vertex) * vertice.size();
	vtxInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vtxInfo.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vtxData{};
	vtxData.pSysMem = vertice.data();

	LOG_HR << G_DXDevice->CreateBuffer(&vtxInfo, &vtxData, &mVertexBuffer);

	//indices
	D3D11_BUFFER_DESC indexInfo{};
	indexInfo.Usage = D3D11_USAGE_DEFAULT;
	indexInfo.ByteWidth = sizeof(uint) * indice.size();
	indexInfo.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexInfo.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData{};
	indexData.pSysMem = indice.data();

	LOG_HR << G_DXDevice->CreateBuffer(&indexInfo, &indexData, &mIndexBuffer);

	mIndiceNum = (uint)indice.size();
	mHasBuffer = true;
	mMesh.SetIndexedVertices(vertice.data(), indice.data(), indice.size());
}

void DXModel::CreateBuffer(Vertex* pVertices, uint vertexsize, uint* pIndices, uint indicesSize)
{
	D3D11_BUFFER_DESC vtxInfo{};
	vtxInfo.Usage = D3D11_USAGE_DEFAULT;
	vtxInfo.ByteWidth = sizeof(Vertex) * vertexsize;
	vtxInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vtxInfo.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vtxData{};
	vtxData.pSysMem = pVertices;

	LOG_HR << G_DXDevice->CreateBuffer(&vtxInfo, &vtxData, &mVertexBuffer);

	D3D11_BUFFER_DESC indexInfo{};
	indexInfo.Usage = D3D11_USAGE_DEFAULT;
	indexInfo.ByteWidth = sizeof(uint) * indicesSize;
	indexInfo.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexInfo.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData{};
	indexData.pSysMem = pIndices;

	LOG_HR << G_DXDevice->CreateBuffer(&indexInfo, &indexData, &mIndexBuffer);

	mIndiceNum = indicesSize;
	mHasBuffer = true;
	mMesh.SetIndexedVertices(pVertices, pIndices, indicesSize);
}

void DXModel::Bind()
{
	uint stride = sizeof(Vertex);
	uint offset = 0;

	G_DXContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	G_DXContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	G_DXContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DXModel::Render()
{
	uint stride = sizeof(Vertex);
	uint offset = 0;

	G_DXContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	G_DXContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	G_DXContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	G_DXContext->DrawIndexed(mIndiceNum, 0, 0);
}
