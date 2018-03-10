#include <PCH.h>
#include <Graphics/GL4/GLModel.h>
#include <Graphics/GL4/GLConfig.h>

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

void GLModel::CreateBuffer(Vertex* pVertices, uint vertexSize, uint* pIndices, uint indicesSize)
{
	//create buffer
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	//start vao grab
	glBindVertexArray(vao);
	//vertices
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexSize, pVertices, GL_STATIC_DRAW);
	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indicesSize, pIndices, GL_STATIC_DRAW);

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

	mIndiceNum = indicesSize;
	mHasBuffer = true;
	mMesh.SetIndexedVertices(pVertices, pIndices, indicesSize);
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