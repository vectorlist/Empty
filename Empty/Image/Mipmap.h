#pragma once


struct R8
{
	uint8 r;
};

struct RGB8
{
	uint8 r, g, b;
};

struct RGBA8
{
	uint8 r, g, b, a;
};

typedef std::unique_ptr<uint8[]>	PixelArray;

struct PixelBuffer
{
	uint8* pData;
	uint width;
	uint height;
	uint bytePerPixel;
};

class Mipmap
{
public:
	Mipmap();
	~Mipmap();

	bool GenerateMipmap(void* pixels, uint width, uint height, uint bytePerPixel);
	PixelBuffer* GetPixelBuffer(uint level);
private:
	PixelArray	RescalePixels(uint8* srcPixels, uint srcW, uint srcH, uint dstW, uint dstH);
	uint		GetTotalMipmapByte(uint witdh, uint height, uint bytePerPixel);
	std::vector<PixelBuffer>	mPixelBuffers;
	PixelArray					mTotalPixels;
	uint						mBytePerPixel;
};

class Filter
{
public:
	static float GuassianVertHori(float x, float y, float alpha = 2.0f);
	//TODO BOX FILTER
};

//PBRT v3
inline float Filter::GuassianVertHori(float x, float y, float alpha)
{
	return std::max(0.0f, expf(-alpha * x * x) - expf(-alpha * y * y));
}

