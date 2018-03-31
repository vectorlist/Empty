#include <Core/PCH.h>
#include <Cache/TextureCache.h>
#include <Graphics/Context.h>
#include <Graphics/GL4/GLTexture.h>
#include <Graphics/DX11/DXTexture.h>
#include <Core/FileSystem.h>

std::unordered_map<std::string, std::shared_ptr<Texture>> TextureCache::mTextures;
std::unordered_map<uint, std::shared_ptr<Texture>> TextureCache::mInternalTexture;

uint TextureCache::mInternalTextureCount = 0;

Texture* TextureCache::LoadTexture(const std::string& filename)
{
	auto found = mTextures.find(filename);
	if (found != mTextures.end()) {
		return found->second.get();
	}

	bool IsDDS = FileSystem::IsSupportedFormat(filename, ".dds");

	std::shared_ptr<Texture> texture = nullptr;

	switch (GContext->GetApiType())
	{
	case GraphicAPI::OPENGL45:
		texture = std::make_shared<GLTexture>();
		if (IsDDS)
			texture->InitFromDDS(filename);
		else
			texture->InitFromImage(filename);
		break;
	case GraphicAPI::DIRECTX11:
		texture = std::make_shared<DXTexture>();
		if (IsDDS)
			texture->InitFromDDS(filename);
		else
			texture->InitFromImage(filename);
		break;
	default:
		break;
	}

	mTextures.insert(std::make_pair(filename, texture));

	return texture.get();
}

Texture* TextureCache::CreateTexture(TextureCreateInfo& info)
{
	//check previous textures
	auto found = mInternalTexture.find(mInternalTextureCount);

	if (found != mInternalTexture.end()) {
		return static_cast<Texture*>(found->second.get());
	}

	//couldnt found
	std::shared_ptr<Texture> texture = nullptr;
	switch (GContext->GetApiType())
	{
	case GraphicAPI::OPENGL45:
		//TODO
		texture = std::make_shared<GLTexture>();
		texture->Init(info);
		mInternalTextureCount++;
		break;
	case GraphicAPI::DIRECTX11:
		texture = std::make_shared<DXTexture>();
		//TODO check success 
		texture->Init(info);
		mInternalTextureCount++;
		break;
	default:
		return nullptr;
	}
	mInternalTexture.insert(std::make_pair(mInternalTextureCount, texture));
	return static_cast<Texture*>(texture.get());
}
