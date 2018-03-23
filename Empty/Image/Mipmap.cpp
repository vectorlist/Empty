#include <PCH.h>

#include <Image/Mipmap.h>
#include <Math/math.h>

Mipmap::Mipmap()
	: mTotalPixels(0),mBytePerPixel(0)
{
}

Mipmap::~Mipmap()
{
}

bool Mipmap::GenerateMipmap(void* pixels, uint width, uint height, uint bytePerPixel)
{
	if (pixels == nullptr) return false;
	mBytePerPixel = bytePerPixel;
	
	PixelArray srcPixels;
	srcPixels.reset((uint8*)pixels);
	uint srcW = width;
	uint srcH = height;

	//Check Pow2 Recalc size
	if (!math::IsPowOf2(srcW) || !math::IsPowOf2(srcH))
	{
		uint pow2W = math::RoundUpPow2(srcW);
		uint pow2H = math::RoundUpPow2(srcH);

		PixelArray newPixels = RescalePixels((uint8*)pixels, srcW, srcH, pow2W, pow2H);
		srcPixels = std::move(newPixels);

		srcW = pow2W;
		srcH = pow2H;
	}

	uint maxLevel = math::FloorInt(std::max(math::LogF2((float)srcW), math::LogF2((float)srcH))) + 1;

	uint totalByte = GetTotalMipmapByte(srcW, srcH, mBytePerPixel);

	mTotalPixels = std::make_unique<uint8[]>(totalByte);
	memcpy(mTotalPixels.get(), srcPixels.get(), sizeof(uint8) * srcW * srcH * mBytePerPixel);

	mPixelBuffers = { { mTotalPixels.get(), srcW, srcH, mBytePerPixel } };

	for (int i = 1; i < maxLevel; ++i)
	{
		PixelBuffer* buffer = &mPixelBuffers[i - 1];

		uint dstW = std::max(1U, buffer->width >> 1);
		uint dstH = std::max(1U, buffer->width >> 1);

		PixelArray dstPixels = this->RescalePixels(buffer->pData, buffer->width, buffer->height, dstW, dstH);

		auto dPtr = dstPixels.get();

		uint offset = buffer->width * buffer->height * mBytePerPixel;
		uint8* startPoint = buffer->pData + offset;

		memcpy(startPoint, dstPixels.get(), sizeof(uint8) * dstW * dstH * mBytePerPixel);

		PixelBuffer dstBuffer;
		dstBuffer.pData = startPoint;
		dstBuffer.width = dstW;
		dstBuffer.height = dstH;
		dstBuffer.bytePerPixel = mBytePerPixel;
		mPixelBuffers.emplace_back(dstBuffer);

	}


	return true;
}

PixelArray Mipmap::RescalePixels(uint8* srcPixels, uint srcW, uint srcH, uint dstW, uint dstH)
{
	float filterWidth = floor(std::max(2.0f, std::max(
		(float)srcW / (float)dstW,
		(float)srcH / (float)dstH)));

	int nSamples = math::FloorInt(filterWidth) * 2;

	float* sSampleWeight = new float[dstW * nSamples];
	int* sSampleIndex = new int[dstW];
	/*S*/
	for (int s = 0; s < dstW; ++s)
	{
		float center = ((float)s + 0.5f) / dstW * srcW;
		sSampleIndex[s] = math::FloorInt(center - filterWidth + 0.5f);

		float weightSum = 0.0f;
		int wOffset = s * nSamples;
		for (int i = 0; i < nSamples; ++i)
		{
			float p = sSampleIndex[s] + 0.5f + i;
			sSampleWeight[wOffset + i] = Filter::GuassianVertHori(p - center, filterWidth);
			weightSum += sSampleWeight[wOffset + i];
		}
		float invW = 1.0f / weightSum;
		for (int i = 0; i < nSamples; ++i)
		{
			sSampleWeight[wOffset + i] *= invW;
		}
	}

	/*T*/
	float *tSampleWeight = new float[dstH * nSamples];
	int* tSampleIndex = new int[dstH];
	for (int t = 0; t < dstH; ++t)
	{
		float center = ((float)t + 0.5f) / dstH * srcH;
		tSampleIndex[t] = math::FloorInt(center - filterWidth + 0.5f);

		float weightSum = 0.0f;
		int wOffset = t * nSamples;
		for (int i = 0; i < nSamples; ++i)
		{
			float p = tSampleIndex[t] + 0.5f + i;
			tSampleWeight[wOffset + i] = Filter::GuassianVertHori(p - center, filterWidth);
			weightSum += tSampleWeight[wOffset + i];
		}
		float invW = 1.0f / weightSum;
		for (int i = 0; i < nSamples; ++i)
		{
			tSampleWeight[wOffset + i] *= invW;
		}
	}

	/*BUILD NEW PIXELS*/
	PixelArray dstPtr = PixelArray(new uint8[dstW * dstH * mBytePerPixel]);
	memset(dstPtr.get(), 0, sizeof(uint8) * dstW * dstH * mBytePerPixel);
	uint8* dstP = dstPtr.get();

	R8*		dstP1 = (R8*)dstPtr.get();
	RGB8*	dstP3 = (RGB8*)dstPtr.get();
	RGBA8*	dstP4 = (RGBA8*)dstPtr.get();
	R8*		srcP1 = (R8*)srcPixels;
	RGB8*	srcP3 = (RGB8*)srcPixels;
	RGBA8*	srcP4 = (RGBA8*)srcPixels;

	for (int t = 0; t < dstH; ++t)
	{
		for (int s = 0; s < dstW; ++s)
		{
			int dstIndex = t * dstW + (s);
			for (int i = 0; i < nSamples; ++i)
			{
				int srcT = math::ClampInt(tSampleIndex[t] + i, 0, srcH - 1);
				for (int j = 0; j < nSamples; ++j)
				{
					int srcS = math::ClampInt(sSampleIndex[s] + j, 0, srcW - 1);
					int srcIndex = srcT * srcW + (srcS);
					float w = tSampleWeight[t * nSamples + i] * sSampleWeight[s * nSamples + j];
					switch (mBytePerPixel)
					{
					case 1:
						dstP1[dstIndex].r += w * srcP1[srcT * srcW + srcS].r;
						break;
					case 2:
						//incomplete
						break;
					case 3:
						dstP3[dstIndex].r += w * srcP3[srcIndex].r;
						dstP3[dstIndex].g += w * srcP3[srcIndex].g;
						dstP3[dstIndex].b += w * srcP3[srcIndex].b;
						break;
					case 4:
						dstP4[dstIndex].r += w * srcP4[srcIndex].r;
						dstP4[dstIndex].g += w * srcP4[srcIndex].g;
						dstP4[dstIndex].b += w * srcP4[srcIndex].b;
						dstP4[dstIndex].a += w * srcP4[srcIndex].a;
						break;
					default:
						break;
					}
					
				}
			}

		}
	}

	delete[] sSampleIndex;
	delete[] sSampleWeight;
	delete[] tSampleIndex;
	delete[] tSampleWeight;
	return std::move(dstPtr);

}

PixelBuffer* Mipmap::GetPixelBuffer(uint level)
{
	if (mPixelBuffers.size() == 0) return nullptr;
	uint index = std::max(0U, std::min(level, mPixelBuffers.size() - 1));

	return &mPixelBuffers[index];
}

uint Mipmap::GetTotalMipmapByte(uint width, uint height, uint bytePerPixel)
{
	uint nBytes = width * height * bytePerPixel;

	while (width > 1 && height > 1)
	{
		width = std::max(1U, width >> 1);
		height = std::max(1U, height >> 1);
		nBytes += width * height * bytePerPixel;
	}

	return nBytes;
}

