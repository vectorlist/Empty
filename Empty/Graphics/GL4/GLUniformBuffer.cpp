#include <Core/PCH.h>
#include <Graphics/GL4/GLUniformBuffer.h>
#include <Graphics/GL4/GLConfig.h>

GLUnifomBuffer::GLUnifomBuffer()
	: ubo(0)
{
	
}

GLUnifomBuffer::~GLUnifomBuffer()
{
	if (ubo) {
		glDeleteBuffers(1, &ubo);
	}
}

void GLUnifomBuffer::Init(BufferCreateInfo* info)
{
	mSize = info->size;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	switch (info->type)
	{
	case BufferType::BUFFER_STATIC:
		glBufferData(GL_UNIFORM_BUFFER, info->size, info->pData, GL_STATIC_DRAW);
		break;
	case BufferType::BUFFER_DYNAMIC:
		glBufferData(GL_UNIFORM_BUFFER, info->size, info->pData, GL_STATIC_DRAW);
		break;
	default:
		glBufferData(GL_UNIFORM_BUFFER, info->size, info->pData, GL_DYNAMIC_DRAW);
		break;
	}
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, mSize);
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);
	
}

void GLUnifomBuffer::Update(const void *data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glNamedBufferSubDataEXT(ubo, 0, mSize, data);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, mSize, data);
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);
}

void GLUnifomBuffer::UpdateRanged(uint offset, uint size, const void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glNamedBufferSubDataEXT(ubo, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);
}

void GLUnifomBuffer::BindVS(uint index)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, ubo);
}

inline void GLUnifomBuffer::BindPS(uint index)
{
	//TODO : Seperate VS and PS uniform block
	glBindBufferBase(GL_UNIFORM_BUFFER, index, ubo);
}
