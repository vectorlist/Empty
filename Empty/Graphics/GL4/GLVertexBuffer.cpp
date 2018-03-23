#include <PCH.h>

#include <Graphics/GL4/GLVertexBuffer.h>
#include <Graphics/GL4/GLConfig.h>
#include <Graphics/TypeTransform.h>

GLVertexBuffer::GLVertexBuffer()
	: vao(0), vbo(0)
{
}

GLVertexBuffer::~GLVertexBuffer()
{
	if (vao) glDeleteVertexArrays(1, &vao);
	if (vbo) glDeleteVertexArrays(1, &vbo);
}

void GLVertexBuffer::Init(VertexBufferCreateInfo* info)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (info->Type == BufferType::BUFFER_STATIC || info->Type == BufferType::BUFFER_DEFAULT) {
		glBufferData(GL_ARRAY_BUFFER, info->VerticesSize, info->pVertices, GL_STATIC_DRAW);
	}
	else {
		glBufferData(GL_ARRAY_BUFFER, info->VerticesSize, info->pVertices, GL_DYNAMIC_DRAW);
	}

	for (uint i = 0; i < info->AttribSize; ++i)
	{
		auto& attrib = info->pAttrib[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attrib.FormatSize, GLTransform::GetVertextFormat(attrib.Format),
			GL_FALSE, attrib.Stride, (void*)attrib.Offset);
	}

	glBindVertexArray(0);
}

void GLVertexBuffer::SubData(uint offset, uint size, void * data)
{
	//TODO : map and un map
}

void GLVertexBuffer::Bind()
{
	glBindVertexArray(vao);
}

void GLVertexBuffer::UnBind()
{
	glBindVertexArray(0);
}
