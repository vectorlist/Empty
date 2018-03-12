#include <PCH.h>

#include <Image/Mipmap.h>
#include <Math/math.h>

Mipmap::Mipmap()
{
}

bool Mipmap::GenerateMipmap(void* pixels, uint width, uint height, uint bytePerPixel)
{
	mBytePerPixel = bytePerPixel;
	//RGB for now and level 0 for now
	uint srcW = width;
	uint srcH = height;

	//uint8* srcPixel = (uint8*)pixels;

	//Check Pow2 Recalc size
	if (!math::IsPowOf2(srcW) || !math::IsPowOf2(srcH))
	{
		uint pow2W = math::RoundUpPow2(srcW);
		uint pow2H = math::RoundUpPow2(srcH);

		//prepare new pixels
		PixelArray newPixels = RescalePixels((uint8*)pixels, srcW, srcH, pow2W, pow2H);
		//auto p = ReP<RGBA8>((RGBA8*)pixels, srcW, srcH, pow2W, pow2H);
		mPixels = std::move(newPixels);
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
	R8*		srcP1 = (R8*)srcPixels;
	RGB8*	srcP3 = (RGB8*)srcPixels;
	RGBA8*	srcP4 = (RGBA8*)srcPixels;

	for (int t = 0; t < dstH; ++t)
	{
		for (int s = 0; s < dstW; ++s)
		{
			int dstIndex = t * dstW + (s * mBytePerPixel);
			for (int i = 0; i < nSamples; ++i)
			{
				int srcT = math::ClampInt(tSampleIndex[t] + i, 0, srcH - 1);
				for (int j = 0; j < nSamples; ++j)
				{
					int srcS = math::ClampInt(sSampleIndex[s] + j, 0, srcW - 1);
					int srcIndex = srcT * srcW + srcS;
					float w = tSampleWeight[t * nSamples + i] * sSampleWeight[s * nSamples + j];
					switch (mBytePerPixel)
					{
					case 1:
						dstP[dstIndex] += w * srcP1[srcT * srcW + srcS].r;
						break;
					case 2:
						//incomplete
						break;
					case 3:
						dstP[dstIndex + 0] += w * srcP3[srcIndex].r;
						dstP[dstIndex + 1] += w * srcP3[srcIndex].g;
						dstP[dstIndex + 2] += w * srcP3[srcIndex].b;
						break;
					case 4:
						dstP[dstIndex + 0] += w * srcP4[srcIndex].r;
						dstP[dstIndex + 1] += w * srcP4[srcIndex].g;
						dstP[dstIndex + 2] += w * srcP4[srcIndex].b;
						dstP[dstIndex + 3] += w * srcP4[srcIndex].a;
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
