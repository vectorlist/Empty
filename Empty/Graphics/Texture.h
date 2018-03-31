#pragma once

#include <core/type.h>

//TODO Seperate Ssampler

class Texture
{
public:
	Texture(){}
	virtual~Texture(){}

	virtual void InitFromDDS(const std::string& filename) = 0;
	virtual void InitFromImage(const std::string& filename) = 0;

	//TODO : binary byte for now (R8)
	//TODO : From Bitmap data or dds
	virtual void Init(TextureCreateInfo& info) { /* do nothing*/ };
	virtual void Bind(uint index) = 0;

	uint width;
	uint height;
};