#pragma once

#include <Graphics/Texture.h>

class GLTexture : public Texture
{
public:
	GLTexture();
	virtual~GLTexture();

	void InitFromFile(const std::string& filename) override;
	void InitFromImage(const std::string& filename) override;
	void Init(TextureCreateInfo& info) override;		//TODO Fix
	void Bind(uint index) override;

	uint mTexture;
};

//#define GL_RED 0x1903
//#define GL_GREEN 0x1904
//#define GL_BLUE 0x1905
//#define GL_ALPHA 0x1906
//#define GL_RG8UI 0x8238
//#define GL_RG16UI 0x823A
//#define GL_RGBA8UI 0x8D7C