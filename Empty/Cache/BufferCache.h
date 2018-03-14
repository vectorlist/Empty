#pragma once

#include <Graphics/DX11/DXConstantBuffer.h>
#include <Graphics/GL4/GLUniformBuffer.h>

#include <Graphics/VertexBuffer.h>
//TODO : remove template
class BufferCache
{
public:
	BufferCache(){}
	//TODO : Intergration all Buffer one CreateBuffer to Buffer
	static UniformBuffer* CreateUniformBuffer(BufferCreateInfo& info);
	static VertexBuffer* CreateVertexBuffer(BufferCreateInfo& info);

	static std::vector<std::shared_ptr<Buffer>> mUniformBuffers;
	static std::vector<std::shared_ptr<Buffer>> mVertexBuffers;
};


