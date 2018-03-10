#pragma once

#include <Graphics/Model.h>

class GLModel : public Model
{
public:
	GLModel();
	virtual~GLModel();

	void CreateBuffer(std::vector<Vertex> &vertice, std::vector<uint> &indice) override;
	void CreateBuffer(Vertex* pVertices, uint vertexSize, uint* pIndices, uint indicesSize) override;
	void Bind() override;
	void Render() override;

private:
	uint vao;
	uint vbo;
	uint ibo;
};