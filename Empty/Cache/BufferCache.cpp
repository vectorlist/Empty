#include <PCH.h>
#include <Cache/BufferCache.h>
#include <Graphics/Context.h>

#include <Graphics/DX11/DXConstantBuffer.h>
#include <Graphics/GL4/GLUniformBuffer.h>

#include <Graphics/GL4/GLVertexBuffer.h>
#include <Graphics/DX11/DXVertexBuffer.h>

std::vector<std::shared_ptr<Buffer>> BufferCache::mUniformBuffers;
std::vector<std::shared_ptr<Buffer>> BufferCache::mVertexBuffers;
std::vector<std::shared_ptr<Buffer>> BufferCache::mIndexBuffers;

UniformBuffer* BufferCache::CreateUniformBuffer(BufferCreateInfo& info)
{
	if (info.binding != BufferBinding::BIND_UNIFORM) {
		ASSERT_MSG(0, "be sure uniform biding type");
	}
	std::shared_ptr<UniformBuffer> ubo = nullptr;
	switch (GContext->GetApiType())
	{
	case GraphicAPI::OPENGL45:
		ubo = std::make_shared<GLUnifomBuffer>();
		ubo->Init(info);
		break;
	case GraphicAPI::DIRECTX11:
		ubo = std::make_shared<DXConstantBuffer>();
		ubo->Init(info);
		break;
	}
	//TODO replace vector to HASH MAP
	mUniformBuffers.emplace_back(ubo);
	return ubo.get();
}

VertexBuffer* BufferCache::CreateVertexBuffer(VertexBufferCreateInfo* info)
{
	std::shared_ptr<VertexBuffer> vbo = nullptr;
	switch (GContext->GetApiType())
	{
	case GraphicAPI::OPENGL45:
		vbo = std::make_shared<GLVertexBuffer>();
		vbo->Init(info);
		break;
	case GraphicAPI::DIRECTX11:
		vbo = std::make_shared<DXVertexBuffer>();
		vbo->Init(info);
		break;
	}
	//TODO replace vector to HASH MAP
	mVertexBuffers.emplace_back(vbo);
	return vbo.get();
}

IndexBuffer* BufferCache::CreateIndexBuffer(IndexBufferCreateInfo* info)
{
	std::shared_ptr<IndexBuffer> ibo = nullptr;
	switch (GContext->GetApiType())
	{
	case GraphicAPI::OPENGL45:
		ibo = std::make_shared<GLIndexBuffer>();
		ibo->Init(info);
		break;
	case GraphicAPI::DIRECTX11:
		ibo = std::make_shared<DXIndexBuffer>();
		ibo->Init(info);
		break;
	}
	
	mIndexBuffers.emplace_back(ibo);
	return ibo.get();
}