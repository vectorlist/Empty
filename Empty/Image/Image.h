#pragma once

#include <core/type.h>

#define FOURCC_DXT1  0x31545844 //(MAKEFOURCC('D','X','T','1'))
#define FOURCC_DXT3  0x33545844 //(MAKEFOURCC('D','X','T','3'))
#define FOURCC_DXT5  0x35545844 //(MAKEFOURCC('D','X','T','5'))

class Image
{
public:
	static void LoadDDSFromFileToGL(const std::string& filename, GLTexture* texture);
};


struct GLDDS
{
	uint height;
	uint width;
	uint linearSize;
	uint mipmapNum;
	uint format;								//FourCC type
	unsigned char* buffer;						//must be free
	uint bufferSize;
};