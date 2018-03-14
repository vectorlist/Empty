#include <PCH.h>
#include "DXModel.h"
#include <Graphics/DX11/DXContext.h>
#include <Graphics/TypeTransform.h>

DXModel::DXModel()
	: mVertexBuffer(nullptr), mIndexBuffer(nullptr), mInputLayout(nullptr)
{

}

DXModel::~DXModel()
{
	SAFE_RELEASE(mVertexBuffer);
	SAFE_RELEASE(mIndexBuffer);
}

/*Create Buffer Without Input Layout and Specific Vertex Type*/
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
	mStride = sizeof(Vertex);
	mMesh.SetIndexedVertices(vertice.data(), indice.data(), indice.size());
}

/*Create Buffer With Input Layout*/
void DXModel::CreateBuffer(ModelCreateInfo& info)
{
	D3D11_BUFFER_DESC vtxInfo{};
	vtxInfo.Usage = D3D11_USAGE_DEFAULT;
	vtxInfo.ByteWidth = info.verticesSize;
	vtxInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vtxInfo.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vtxData{};
	vtxData.pSysMem = info.pVertices;

	LOG_HR << G_DXDevice->CreateBuffer(&vtxInfo, &vtxData, &mVertexBuffer);

	//indices
	D3D11_BUFFER_DESC indexInfo{};
	indexInfo.Usage = D3D11_USAGE_DEFAULT;
	indexInfo.ByteWidth = info.indicesSize;
	indexInfo.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexInfo.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData{};
	indexData.pSysMem = info.pIndices;

	LOG_HR << G_DXDevice->CreateBuffer(&indexInfo, &indexData, &mIndexBuffer);

	mIndiceNum = info.indicesSize;
	mHasBuffer = true;
	mStride = info.pInputAttrib[0].stride;
	//Create Dummy InputLayout
	this->CreateInputLayout(info);
}

void DXModel::CreateInputLayout(ModelCreateInfo& info)
{
	//Dummy Vertex code for InputLayout
	std::string dummyCode;

	//match to inputlayout
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements(info.inputAttibSize);

	dummyCode += "struct VS_In {";

	for (uint i = 0; i < info.inputAttibSize; ++i)
	{
		D3D11_INPUT_ELEMENT_DESC& element = inputElements[i];
		VertexInputAttib& attr = info.pInputAttrib[i];
		//Match Semantic Name (0 index must be "POSITION", others "TEXCOORD0", "TEXCOORDS1"...etc) 
		element.SemanticName = (i == 0) ? "POSITION" : "TEXCOORD";
		element.SemanticIndex = (i == 0) ? 0 : (i - 1);
		element.Format = DXTransform::GetVertexFormat(attr.format, attr.formatSize);
		element.InputSlot = attr.location;
		element.AlignedByteOffset = attr.offset;
		element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;		//TODO : INSTANCE
		element.InstanceDataStepRate = 0;							//TODO : INSTACE PER DRAW

		uint formatSize = attr.formatSize;

		//TODO : append float size
		if (i == 0) {
			dummyCode += "float" + std::to_string(formatSize) + " pos : POSITION; ";
		}
		else {
			dummyCode += "float" + std::to_string(formatSize) +
				" param" + std::to_string(i - 1) + " : TEXCOORD" + std::to_string(i - 1) + ";";
		}
	}

	dummyCode += "}; float4 VS(VS_In vs) : SV_POSITION { return vs.pos.xxxx;};";

	ID3DBlob* errBlob = nullptr;
	ID3DBlob* vBlob = nullptr;
	LOG_HR << D3DCompile(dummyCode.c_str(), dummyCode.size(), nullptr, nullptr,
		nullptr, "VS", "vs_5_0", 0, 0, &vBlob, &errBlob);

	if (errBlob) {
		LOG << errBlob->GetBufferPointer() << ENDN;
		errBlob->Release();
	}

	//Create Layout
	LOG_HR << G_DXDevice->CreateInputLayout(
		inputElements.data(),
		inputElements.size(),
		vBlob->GetBufferPointer(),
		vBlob->GetBufferSize(),
		&mInputLayout);

	//Remove Dummy Shader
	vBlob->Release();
}


void DXModel::Bind()
{
	if (mInputLayout != nullptr)
		G_DXContext->IASetInputLayout(mInputLayout);
	G_DXContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &mStride, &mOffset);
	G_DXContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	G_DXContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DXModel::Render()
{
	if (mInputLayout != nullptr)	//InputLayout Created?
		G_DXContext->IASetInputLayout(mInputLayout);
	G_DXContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &mStride, &mOffset);
	G_DXContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	G_DXContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	G_DXContext->DrawIndexed(mIndiceNum, 0, 0);
}


