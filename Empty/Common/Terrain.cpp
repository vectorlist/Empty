#include <PCH.h>
#include <Common/Terrain.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <fstream>
#include <ostream>

Terrain::Terrain()
{
}

void Terrain::InitFromBitMap(const char* filename)
{
	FILE* filePtr;
	unsigned long long count;
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfoHeader;

	uint8* bitmapImage;
	uint8 height;

	
}
