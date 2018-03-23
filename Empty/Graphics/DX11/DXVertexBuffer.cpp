#include <PCH.h>
#include <Graphics/DX11/DXVertexBuffer.h>
#include <Graphics/DX11/DXContext.h>
#include <Graphics/TypeTransform.h>

DXVertexBuffer::DXVertexBuffer()
	: mVbo(nullptr), mInputLayout(nullptr), mStride(0), mOffset(0)
{
}

DXVertexBuffer::~DXVertexBuffer()
{
	SAFE_RELEASE(mVbo);
	SAFE_RELEASE(mInputLayout);
}

void DXVertexBuffer::Init(VertexBufferCreateInfo* info)
{
	D3D11_BUFFER_DESC bufferInfo{};
	bufferInfo.Usage = info->Type != BufferType::BUFFER_DYNAMIC ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
	bufferInfo.ByteWidth = info->VerticesSize;
	bufferInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferInfo.CPUAccessFlags = 0;
	bufferInfo.MiscFlags = 0;
	bufferInfo.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA bufferData{};
	bufferData.pSysMem = info->pVertices;

	LOG_HR << G_DXDevice->CreateBuffer(&bufferInfo, &bufferData, &mVbo);

	CreateInputLayout(*info);
	mStride = info->pAttrib[0].Stride;
}

void DXVertexBuffer::CreateInputLayout(VertexBufferCreateInfo& info)
{
	std::string dummyCode;

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements(info.AttribSize);

	dummyCode += "struct VS_In {";

	for (uint i = 0; i < info.AttribSize; ++i)
	{
		D3D11_INPUT_ELEMENT_DESC& element = inputElements[i];
		VertexAttrib& attr = info.pAttrib[i];
		element.SemanticName = (i == 0) ? "POSITION" : "TEXCOORD";
		element.SemanticIndex = (i == 0) ? 0 : (i - 1);
		element.Format = DXTransform::GetVertexFormat(attr.Format, attr.FormatSize);
		element.InputSlot = attr.Location;
		element.AlignedByteOffset = attr.Offset;
		element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;		//TODO : INSTANCE
		element.InstanceDataStepRate = 0;							//TODO : INSTACE PER DRAW

		uint formatSize = attr.FormatSize;
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
	LOG_HR << G_DXDevice->CreateInputLayout(
		inputElements.data(),
		inputElements.size(),
		vBlob->GetBufferPointer(),
		vBlob->GetBufferSize(),
		&mInputLayout);
	vBlob->Release();
}

void DXVertexBuffer::SubData(uint offset, uint size, void * data)
{
}

void DXVertexBuffer::Bind()
{
	G_DXContext->IASetInputLayout(mInputLayout);
	G_DXContext->IASetVertexBuffers(0, 1, &mVbo, &mStride, &mOffset);
}

void DXVertexBuffer::UnBind()
{
	//DX dont need to unbind
}
