#pragma once

#include <Graphics/VertexBuffer.h>
#include <Graphics/UniformBuffer.h>
#include <Graphics/IndexBuffer.h>
#include <Graphics/FrameBuffer.h>

class BufferCache
{
public:
	BufferCache(){}

	static UniformBuffer*	CreateUniformBuffer(BufferCreateInfo* info);
	static VertexBuffer*	CreateVertexBuffer(VertexBufferCreateInfo* info);
	static IndexBuffer*		CreateIndexBuffer(IndexBufferCreateInfo* info);
	static FrameBuffer*     CreateFrameBuffer(FrameBufferCreateInfo* info);

	static std::vector<std::shared_ptr<Buffer>> mUniformBuffers;
	static std::vector<std::shared_ptr<Buffer>> mVertexBuffers;
	static std::vector<std::shared_ptr<Buffer>> mIndexBuffers;
};


