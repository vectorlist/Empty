#pragma once

#include <graphics/model.h>

class ID3D11Buffer;
class DXModel : public Model
{
public:
	DXModel();
	virtual~DXModel();

	void CreateBuffer(std::vector<Vertex> &vertice, std::vector<uint> &indice) override;
	void CreateBuffer(ModelCreateInfo& info) override;
	void Bind() override;
	void Render() override;

	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;

	//optional
	void CreateInputLayout(ModelCreateInfo& info);
	ID3D11InputLayout* mInputLayout;

	//Stride
	uint mStride = 0;
	uint mOffset = 0;
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