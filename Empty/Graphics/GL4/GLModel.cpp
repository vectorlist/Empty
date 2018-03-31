#include <Core/PCH.h>
#include <Graphics/GL4/GLModel.h>
#include <Graphics/GL4/GLConfig.h>
#include <Graphics/TypeTransform.h>
#include <Graphics/Texture.h>

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

	glCreateVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	//VERTEX
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertice.size(), vertice.data(), GL_STATIC_DRAW);
	
	//ATTRIBS
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Vertex::normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Vertex::st));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Vertex::binormal));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Vertex::tangent));

	//OPTIONAL INSTANCING (you can use InstanceVS.glsl or ForwardVS.glsl)
	//Just define instance VBO (updating on rumtime)
	glGenBuffers(1, &instancedVbo);
	glBindBuffer(GL_ARRAY_BUFFER, instancedVbo);

	for (uint i = 0; i < 4; ++i)
	{
		glEnableVertexAttribArray(5 + i);
		glVertexAttribPointer(5 + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4x4), 
			(void*)(sizeof(float) * i * 4));
		//Enable to Instancing
		glVertexAttribDivisor(5 + i, 1);
	}

	//INDEX
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), indices.data(), GL_STATIC_DRAW);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	
	mIndiceNum = (uint)indices.size();
	mHasBuffer = true;
}


void GLModel::Bind()
{
	//only able to vao
	glBindVertexArray(vao);
}

void GLModel::Render()
{
	glBindVertexArray(vao);

	for (uint i = 0; i < mNodes.size(); ++i) 
	{
		if (mNodes[i].texture) {
			mNodes[i].texture->Bind(0);
		}
		/*if (i == 1) continue;*/
		glDrawElements(GL_TRIANGLES, mNodes[i].IndicesNum, GL_UNSIGNED_INT, 
			(void*)(sizeof(uint) * mNodes[i].IndicesOffset));
	}

	glBindVertexArray(0);
}

void GLModel::RenderInstnaced(uint instanceNum, const Matrix4x4* data)
{
	//Upload data to GPU
	glBindBuffer(GL_ARRAY_BUFFER, instancedVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4x4) * instanceNum, data, GL_DYNAMIC_DRAW);

	//Bind VAO
	glBindVertexArray(vao);

	for (uint i = 0; i < mNodes.size(); ++i)
	{
		//TEXTURE PER NODe
		if (mNodes[i].texture) {
			mNodes[i].texture->Bind(0);
		}

		glDrawElementsInstanced(GL_TRIANGLES, mNodes[i].IndicesNum, GL_UNSIGNED_INT,
			(void*)(sizeof(uint) * mNodes[i].IndicesOffset), instanceNum);
	}

	glBindVertexArray(NULL);
}
