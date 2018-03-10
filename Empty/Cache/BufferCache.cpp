#include <PCH.h>
#include <Cache/BufferCache.h>
#include <Graphics/Context.h>

std::vector<std::shared_ptr<Buffer>> BufferCache::mUniformBuffers;

UniformBuffer* BufferCache::CreateUniformBuffer(BufferCreateInfo& info)
{
	if (info.binding != BufferBinding::BIND_UNIFORM) {
		ASSERT_MSG(0, "be sure uniform biding type");
	}
	std::shared_ptr<UniformBuffer> ubo = nullptr;
	switch (G_Context->GetApiType())
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