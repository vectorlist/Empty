#pragma once

#include <graphics/texture.h>

class TextureCache
{
public:
	static Texture* LoadTexture(const std::string& filename);
	static Texture* CreateTexture(TextureCreateInfo& info);

	static std::unordered_map<std::string, std::shared_ptr<Texture>>	mTextures;
	static std::unordered_map<uint, std::shared_ptr<Texture>>			mInternalTexture;
	static uint mInternalTextureCount;
};