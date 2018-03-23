#pragma once

#include <Image/Mipmap.h>

enum class PixelFormat
{
	R8 = 0,
	RG8,
	RGB8,
	RGBA8,
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
	~Image();

	void LoadFromFile(const char* filename, uint requestBytePerPixel = IMAGE_REQ_RGBA);

	bool			IsPow2() const;
	bool			IsSqaure() const;
	PixelFormat		GetFormat() const;
	uint			GetBytePerPixel() const;
	uint			GetWidth() const;
	uint			GetHeight() const;
	std::string		GetFormatStr(PixelFormat format);
	uint8*			GetData();
	PixelBuffer*	GetPixelBuffer();
	Mipmap*			CreateMipmaps();
	uint			GetGraphicsMipmapPossibilityCount() const;
private:
	
	std::unique_ptr<uint8[]>	mPixels;
	PixelBuffer					mPixelBuffer;
	PixelFormat					mFormat;
	uint						mBytePerPixel;
	uint						mWidth;
	uint						mHeight;
	Mipmap						mMipmap;
};
