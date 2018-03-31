#pragma once

#include <graphics/model.h>

//Pre Define instan buffer size
#define		MAX_INSTANCE_PER_MODEL	32768		

class ID3D11Buffer;
class DXModel : public Model
{
public:
	DXModel();
	virtual~DXModel();

	void CreateBuffer(std::vector<Vertex> &vertice, std::vector<uint> &indice) override;
	
	void Bind() override;
	void Render() override;
	void RenderInstnaced(uint instanceNum, const Matrix4x4* data);

	//Instancing (Matrix4x4)
	static ID3D11Buffer* gInstanceBuffer;
private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	ID3D11InputLayout* mInputLayout;
};

//typedef struct D3D11_INPUT_ELEMENT_DESC {
//	LPCSTR                     SemanticName;
//	UINT                       SemanticIndex;
//	DXGI_FORMAT                Format;
//	UINT                       InputSlot;
//	UINT                       AlignedByteOffset;
//	D3D11_INPUT_CLASSIFICATION InputSlotClass;
//	UINT                       InstanceDataStepRate;
//} D3D11_INPUT_ELEMENT_DESC;

//Format
//Type : DXGI_FORMAT
//
//The data type of the element data.See DXGI_FORMAT.
//
//InputSlot
//Type : UINT
//
//An integer value that identifies the input - assembler(see input slot).Valid values are between 0 and 15, defined in D3D11.h.
//
//AlignedByteOffset
//Type : UINT
//
//Optional.Offset(in bytes) between each element.Use D3D11_APPEND_ALIGNED_ELEMENT for convenience to define the current element 
//directly after the previous one, including any packing if necessary.
//
//InputSlotClass
//Type : D3D11_INPUT_CLASSIFICATION
//
//Identifies the input data class for a single input slot(see D3D11_INPUT_CLASSIFICATION).
//
//InstanceDataStepRate
//Type : UINT
//
//The number of instances to draw using the same per - instance data before advancing in the buffer by one element.
//This value must be 0 for an element that contains per - vertex data(the slot class is set to D3D11_INPUT_PER_VERTEX_DATA)

//{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

//D3D11_BUFFER_DESC desc;
//ZeroMemory(&desc, sizeof(desc));
//desc.Usage = D3D11_USAGE_DYNAMIC;
//desc.ByteWidth = size;
//desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//d3dDevice->CreateBuffer(&desc, initialVertexData, &vertexBuffer);
//Now that you have a dynamic vertex buffer, you can update it using ID3D11DeviceContext::Map and ID3D11DeviceContext::Unmap.Example:
//
//D3D11_MAPPED_SUBRESOURCE resource;
//d3dDeviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
//memcpy(resource.pData, sourceData, vertexDataSize);
//d3dDeviceContext->Unmap(vertexBuffer, 0);