#include <PCH.h>
#include <Graphics/VertexBuffer.h>

#include <Graphics/GL4/GLConfig.h>

GLVertexBuffer::GLVertexBuffer()
{
}

GLVertexBuffer::~GLVertexBuffer()
{
}

void GLVertexBuffer::Init(BufferCreateInfo& info)
{
	glGenBuffers(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (info.type == BufferType::BUFFER_STATIC || info.type == BufferType::BUFFER_DEFAULT) {
		glBufferData(GL_ARRAY_BUFFER, info.size, info.pData, GL_STATIC_DRAW);
	}
	else {
		glBufferData(GL_ARRAY_BUFFER, info.size, info.pData, GL_DYNAMIC_DRAW);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

void GLVertexBuffer::SubData(uint offset, uint size, void * data)
{
}

void GLVertexBuffer::SetInputLayout(std::vector<InputLayout>& layout)
{
	//bind vbo
	glBindBuffer(GL_VERTEX_ARRAY, vbo);

	static GLenum(*InputToGL)(InputType) = [](InputType type)->GLenum
	{
		switch (type)
		{
		case InputType::BYTE:
			return GL_BYTE;
		case InputType::INT:
			return GL_INT;
		case InputType::UINT:
			return GL_UNSIGNED_INT;
		case InputType::SHORT:
			return GL_SHORT;
		case InputType::FLOAT:
			return GL_FLOAT;
		case InputType::DOUBLE:
			return GL_DOUBLE;
		default:
			ASSERT_MSG(0, "invalid input type");
			return -1;
		}
	};

	//TODO : gather stride

	
	for (uint i = 0; i < layout.size(); ++i)
	{
		glEnableVertexAttribArray(i);	//enalbe each commponent;
		glVertexAttribPointer(i,
			layout[i].elemintDemension,
			InputToGL(layout[i].elementType), GL_FALSE,
			layout[i].stride,
			(void*)(layout[i].offset));
	}
	glBindBuffer(GL_VERTEX_ARRAY, 0);
}

void GLVertexBuffer::Bind()
{
}
