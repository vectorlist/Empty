#pragma once

#include <Graphics/Model.h>

class GLModel : public Model
{
public:
	GLModel();
	virtual~GLModel();

	void CreateBuffer(std::vector<Vertex> &vertice, std::vector<uint> &indice) override;
	void CreateBuffer(ModelCreateInfo& info) override;
	void Bind() override;
	void Render() override;

private:
	uint vao;
	uint vbo;
	uint ibo;
};