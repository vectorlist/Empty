#pragma once

#include <graphics/model.h>

class ID3D11Buffer;
class DXModel : public Model
{
public:
	DXModel();
	virtual~DXModel();

	void CreateBuffer(std::vector<Vertex> &vertice, std::vector<uint> &indice) override;
	void CreateBuffer(Vertex* pVertices, uint vertexsize, uint* pIndices, uint indicesSize) override;
	virtual void Bind() override;
	virtual void Render() override;

	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
};

