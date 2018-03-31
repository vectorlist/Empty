#pragma once

#include <Graphics/Model.h>

class GLModel : public Model
{
public:
	GLModel();
	virtual~GLModel();

	void CreateBuffer(std::vector<Vertex> &vertice, std::vector<uint> &indice) override;

	void Bind() override;
	void Render() override;
	void RenderInstnaced(uint instanceNum, const Matrix4x4* data) override;

private:
	uint vao;
	uint vbo;
	uint ibo;
	uint instancedVbo;
};