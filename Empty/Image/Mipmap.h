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

class Mipmap
{
public:
	Mipmap();

	bool GenerateMipmap(void* pixels, uint width, uint height, uint bytePerPixel);

	uint mBytePerPixel;

	PixelArray RescalePixels(uint8* srcPixels, uint srcW, uint srcH, uint dstW, uint dstH);

	PixelArray mPixels;
};







class Filter
{
public:
	static float GuassianVertHori(float x, float y, float alpha = 2.0f);
};

//PBRT v3
inline float Filter::GuassianVertHori(float x, float y, float alpha)
{
	return std::max(0.0f, expf(-alpha * x * x) - expf(-alpha * y * y));
}

