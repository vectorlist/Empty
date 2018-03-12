#pragma once

#include <Image/Mipmap.h>

enum class PixelFormat
{
	R8 = 0,
	R16,
	RG8,
	RGB8,
	RGB16,
	RGBA8,
	RGBA16,
	UNKNOWN
};

#define IMAGE_REQ_R					1U
#define IMAGE_REQ_RB				2U
#define IMAGE_REQ_RGB				3U
#define IMAGE_REQ_RGBA				4U

class Image
{
public:
	Image();
	~Image(){}

	void LoadFromFile(const char* filename, uint requestBytePerPixel = IMAGE_REQ_RGBA);

	bool		IsPow2() const;
	PixelFormat GetFormat() const;
	uint		GetBytePerPixel() const;
	uint		GetWidth() const;
	uint		GetHeight() const;
	std::string	GetFormatStr(PixelFormat format);
	uint8*		GetData();
	bool		CreateMipmaps(Mipmap* mipmap);
private:
	//OpenGL , DirectX auto gen mipmap
	uint GetGraphicsMipmapPossibilityCount(uint width, uint height);
	//Force to Gen Mipmap menually by Pixels (with power of 2)
	uint GetPxielsMipmapPossibilityCount(uint width, uint height);
	//std::unique_ptr<RGBA8[]> mPixels;
	std::unique_ptr<uint8[]> mPixels;

	PixelFormat mFormat;
	uint mBytePerPixel;
	uint mWidth;
	uint mHeight;
};
