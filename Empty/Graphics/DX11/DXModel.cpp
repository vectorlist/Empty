#include <Core/PCH.h>
#include "DXModel.h"
#include <Graphics/DX11/DXContext.h>
#include <Graphics/TypeTransform.h>
#include <Graphics/Texture.h>

ID3D11Buffer* DXModel::gInstanceBuffer = nullptr;

DXModel::DXModel()
	: mVertexBuffer(nullptr), mIndexBuffer(nullptr), mInputLayout(nullptr)
{

}

DXModel::~DXModel()
{
	SAFE_RELEASE(mInputLayout);
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
	vtxInfo.MiscFlags = 0;
	vtxInfo.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vtxData{};
	vtxData.pSysMem = vertice.data();

	LOG_HR << G_DXDevice->CreateBuffer(&vtxInfo, &vtxData, &mVertexBuffer);

	D3D11_BUFFER_DESC indexInfo{};
	indexInfo.Usage = D3D11_USAGE_DEFAULT;
	indexInfo.ByteWidth = sizeof(uint) * indice.size();
	indexInfo.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexInfo.CPUAccessFlags = 0;
	indexInfo.MiscFlags = 0;
	indexInfo.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData{};
	indexData.pSysMem = indice.data();

	LOG_HR << G_DXDevice->CreateBuffer(&indexInfo, &indexData, &mIndexBuffer);

	mIndiceNum = (uint)indice.size();
	mHasBuffer = true;

	//Layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//INSTANCING MATRIX
		{ "INSTANCE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		//TODO : INSTANCING
	};

	//start per instance from TEXCOORD4 must be offset reset as 0

	//Dummy Code for proxy layout input
	std::string dummyCode = R"(
struct VS_In {
float3 pos : POSITION;
float3 param0 : TEXCOORD0;
float2 param1 : TEXCOORD1;
float3 param2 : TEXCOORD2;
float3 param3 : TEXCOORD3;
//Matrix instance
float4 ins0 : INSTANCE0;
float4 ins1 : INSTANCE1;
float4 ins2 : INSTANCE2;
float4 ins3 : INSTANCE3;
};
float4 VS(VS_In vs) : SV_POSITION { return vs.pos.xxxx;};
)";
	ID3DBlob* eb = nullptr; //error blob
	ID3DBlob* vb = nullptr; //code blob

	//Compile
	LOG_HR << D3DCompile(dummyCode.c_str(), dummyCode.size(), nullptr, nullptr, nullptr,
		"VS", "vs_5_0", 0, 0, &vb, &eb);

	if (eb != nullptr) {
		ASSERT_MSG(0, "failed to compile");
	}

	LOG_HR << G_DXDevice->CreateInputLayout(
		inputs.data(),
		inputs.size(),
		vb->GetBufferPointer(),
		vb->GetBufferSize(),
		&mInputLayout);

	//TODO : INSTACNING BUFFER (expensive per model static?)
	//Check already created 
	if (DXModel::gInstanceBuffer == nullptr)
	{
		D3D11_BUFFER_DESC instanceInfo{};
		instanceInfo.Usage = D3D11_USAGE_DYNAMIC;
		instanceInfo.ByteWidth = sizeof(Vertex) * MAX_INSTANCE_PER_MODEL;
		instanceInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instanceInfo.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		instanceInfo.StructureByteStride = 0;
		instanceInfo.MiscFlags = 0;

		LOG_HR << G_DXDevice->CreateBuffer(&instanceInfo, nullptr, &gInstanceBuffer);
	}

};



void DXModel::Bind()
{

}

void DXModel::Render()
{
	uint stride = sizeof(Vertex);
	uint offset = 0;
	G_DXContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	G_DXContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	G_DXContext->IASetInputLayout(mInputLayout);
	G_DXContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (uint i = 0; i < mNodes.size(); ++i)
	{
		if (mNodes[i].texture) {
			mNodes[i].texture->Bind(0);
		}
		G_DXContext->DrawIndexed(mNodes[i].IndicesNum, mNodes[i].IndicesOffset, 0);
	}
}

void DXModel::RenderInstnaced(uint instanceNum, const Matrix4x4* data)
{
	//Upload data to GPU
	D3D11_MAPPED_SUBRESOURCE mapped{};
	G_DXContext->Map(DXModel::gInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	//copy ranged
	memcpy(mapped.pData, data, sizeof(Matrix4x4) * instanceNum);
	//close
	G_DXContext->Unmap(DXModel::gInstanceBuffer, 0);

	//main buffer : pos normal st......
	//instance buffer : float4 row0 row1 row2 row3
	
	uint strides[2] = { sizeof(Vertex), sizeof(Matrix4x4) };
	uint offsets[2] = { 0,0 };
	ID3D11Buffer* buffers[2] = { mVertexBuffer, gInstanceBuffer };//main buffer, instance buffer

	G_DXContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	G_DXContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	G_DXContext->IASetInputLayout(mInputLayout);
	G_DXContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (uint i = 0; i < mNodes.size(); ++i)
	{
		//TEXTURE PER NODe
		if (mNodes[i].texture) {
			mNodes[i].texture->Bind(0);
		}

		G_DXContext->DrawIndexedInstanced(mNodes[i].IndicesNum, instanceNum,
			mNodes[i].IndicesOffset, 0, 0);
	}

}


