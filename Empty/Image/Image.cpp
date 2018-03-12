#include <PCH.h>
#include <Image/Image.h>
#include <Math/math.h>

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

Image::Image()
	: mFormat(PixelFormat::UNKNOWN), mWidth(0), mHeight(0), mBytePerPixel(3)
{
}

void Image::LoadFromFile(const char* filename, uint requestBytePerPixel)
{
	ASSERT_MSG(requestBytePerPixel < 5, "invalid byte per pixels");
	int w, h, comp;
	uint8* buffer = stbi_load(filename, &w, &h, &comp, requestBytePerPixel);

	mBytePerPixel = requestBytePerPixel;

	if (!buffer){
		ASSERT_MSG(0, "failed to load image");
	}
	
	PixelFormat format;
	switch (mBytePerPixel)
	{
	case 1:
		format = PixelFormat::R8;
		break;
	case 2:
		format = PixelFormat::RG8;
		break;
	case 3:
		format = PixelFormat::RGB8;
		break;
	case 4:
		format = PixelFormat::RGBA8;
		break;
	default:
		format = PixelFormat::UNKNOWN;
		break;
	}

	LOG << "Image Format : " << GetFormatStr(format) << ENDN;
	LOG << "Width : " << w << ENDN;
	LOG << "Height : " << h << ENDN;
	LOG << "Graphically Mipmap gen count : " << GetGraphicsMipmapPossibilityCount(w, h) << ENDN;
	LOG << "Pixel Mipmap gen count : " << GetPxielsMipmapPossibilityCount(w, h) << ENDN;
	
	mFormat = format;
	mWidth = w;
	mHeight = h;
	//mPixels = std::unique_ptr<uint8[]>(std::move(buffer));
	mPixels = std::make_unique<uint8[]>(w * h * mBytePerPixel);	
	memcpy(mPixels.get(), buffer, w * h * mBytePerPixel);

	STBI_FREE(buffer);
}

PixelFormat Image::GetFormat() const
{
	return mFormat;
}

uint Image::GetBytePerPixel() const
{
	return mBytePerPixel;
}

uint Image::GetWidth() const
{
	return mWidth;
}

uint Image::GetHeight() const
{
	return mHeight;
}

uint8* Image::GetData()
{
	return mPixels.get();
}

uint Image::GetGraphicsMipmapPossibilityCount(uint width, uint height)
{
	uint  mipCount = 1;
	while (width > 1 && height > 1)
	{
		width = std::max((uint)width / 2, 1U);
		height = std::max((uint)height / 2, 1U);
		++mipCount;
	}
	return mipCount;
}

uint Image::GetPxielsMipmapPossibilityCount(uint width, uint height)
{
	uint pow2W = width;
	uint pow2H = height;
	if (!math::IsPowOf2(width) || !math::IsPowOf2(height))
	{
		pow2W = math::RoundUpPow2(width);
		pow2H = math::RoundUpPow2(height);
	}
	return GetGraphicsMipmapPossibilityCount(pow2W, pow2H);
}

bool Image::IsPow2() const
{
	if (!math::IsPowOf2(mWidth) || !math::IsPowOf2(mHeight))
	{
		return false;
	}
	return true;
}

bool Image::CreateMipmaps(Mipmap* mipmap)
{
	if (!mipmap->GenerateMipmap(this->GetData(), mWidth, mHeight, mBytePerPixel)) {
		return false;
	}
	return true;
}

std::string Image::GetFormatStr(PixelFormat format)
{
	switch (format)
	{
	case PixelFormat::R8:
		return "R8";
	case PixelFormat::RG8:
		return "RG8";
		break;
	case PixelFormat::RGB8:
		return "RGB8";
	case PixelFormat::RGBA8:
		return "RG8A8";
	default:
		return "UNKNWON FORMAT";
	}
}