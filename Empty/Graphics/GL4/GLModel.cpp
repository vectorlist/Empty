#include <PCH.h>
#include <Graphics/GL4/GLModel.h>
#include <Graphics/GL4/GLConfig.h>
#include <Graphics/TypeTransform.h>

GLModel::GLModel()
{
}

GLModel::~GLModel()
{
	if (mHasBuffer) {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
	}
}

void GLModel::CreateBuffer(std::vector<Vertex>& vertice, std::vector<uint>& indices)
{
	//create buffer
	//glGenVertexArrays(1, &vao);
	glCreateVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	//start vao grab
	glBindVertexArray(vao);

	//vertices
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertice.size(), vertice.data(), GL_STATIC_DRAW);
	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), indices.data(), GL_STATIC_DRAW);

	//create input layout
	//layout index, data per size, type, normalize? , stride, offset
	//VETEX
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	//NORMAL
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Vertex::normal));

	//ST
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Vertex::st));

	//BINORMAL
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Vertex::binormal));

	//TANGENT
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Vertex::tangent));

	//close VAO
	glBindVertexArray(0);
	
	mIndiceNum = (uint)indices.size();
	mHasBuffer = true;
	mMesh.SetIndexedVertices(vertice.data(), indices.data(), indices.size());
}

void GLModel::CreateBuffer(ModelCreateInfo& info)
{

	glCreateVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, info.verticesSize, info.pVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indicesSize, info.pIndices, GL_STATIC_DRAW);

	for (int i = 0; i < info.inputAttibSize; ++i)
	{
		glEnableVertexAttribArray(i);
		VertexInputAttib attb = info.pInputAttrib[i];
		glVertexAttribPointer(i,
			attb.formatSize,
			GLTransform::GetVertextFormat(attb.format),
			GL_FALSE,
			attb.stride,
			(void*)attb.offset);
	}

	//close VAO
	glBindVertexArray(0);

	mIndiceNum = info.indicesSize;
	mHasBuffer = true;
	//TODO bool possible mesh
	//mMesh.SetIndexedVertices(vertice.data(), indices.data(), indices.size());
}

void GLModel::Bind()
{
	//only able to vao
	glBindVertexArray(vao);
}

void GLModel::Render()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, mIndiceNum, GL_UNSIGNED_INT, nullptr);	//bind vao already

	glBindVertexArray(0);
}
