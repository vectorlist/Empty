#pragma once

//TODO Terrain comnpare (BSP, QUADTREE, BVH) trees

class Terrain
{
public:
	//TODO : intt from file/ init from binary / init form noise / init from model
	Terrain();

	void InitFromBitMap(const char* filename);
	//void InitFromImage(void* image);
};

//int stringLength;
//ifstream fin;
//char input;
//
//// Initialize the string that will hold the terrain file name.
//stringLength = 256;
//m_terrainFilename = new char[stringLength];
//if (!m_terrainFilename)
//{
//	return false;
//}
//
//// Open the setup file.  If it could not open the file then exit.
//fin.open(filename);
//if (fin.fail())
//{
//	return false;
//}
//
//// Read up to the terrain file name.
//fin.get(input);
//while (input != ':')
//{
//	fin.get(input);
//}
//
//// Read in the terrain file name.
//fin >> m_terrainFilename;
//
//// Read up to the value of terrain height.
//fin.get(input);
//while (input != ':')
//{
//	fin.get(input);
//}
//
//// Read in the terrain height.
//fin >> m_terrainHeight;
//
//// Read up to the value of terrain width.
//fin.get(input);
//while (input != ':')
//{
//	fin.get(input);
//}
//
//// Read in the terrain width.
//fin >> m_terrainWidth;
//
//// Read up to the value of terrain height scaling.
//fin.get(input);
//while (input != ':')
//{
//	fin.get(input);
//}
//
//// Read in the terrain height scaling.
//fin >> m_heightScale;
//
//// Close the setup file.
//fin.close();
//
//return true;

//int error, imageSize, i, j, k, index;
//FILE* filePtr;
//unsigned long long count;
//BITMAPFILEHEADER bitmapFileHeader;
//BITMAPINFOHEADER bitmapInfoHeader;
//unsigned char* bitmapImage;
//unsigned char height;
//
//
//// Start by creating the array structure to hold the height map data.
//m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
//if (!m_heightMap)
//{
//	return false;
//}
//
//// Open the bitmap map file in binary.
//error = fopen_s(&filePtr, m_terrainFilename, "rb");
//if (error != 0)
//{
//	return false;
//}
//
//// Read in the bitmap file header.
//count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
//if (count != 1)
//{
//	return false;
//}
//
//// Read in the bitmap info header.
//count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
//if (count != 1)
//{
//	return false;
//}
//
//// Make sure the height map dimensions are the same as the terrain dimensions for easy 1 to 1 mapping.
//if ((bitmapInfoHeader.biHeight != m_terrainHeight) || (bitmapInfoHeader.biWidth != m_terrainWidth))
//{
//	return false;
//}
//
//// Calculate the size of the bitmap image data.  
//// Since we use non-divide by 2 dimensions (eg. 257x257) we need to add an extra byte to each line.
//imageSize = m_terrainHeight * ((m_terrainWidth * 3) + 1);
//
//// Allocate memory for the bitmap image data.
//bitmapImage = new unsigned char[imageSize];
//if (!bitmapImage)
//{
//	return false;
//}
//
//// Move to the beginning of the bitmap data.
//fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
//
//// Read in the bitmap image data.
//count = fread(bitmapImage, 1, imageSize, filePtr);
//if (count != imageSize)
//{
//	return false;
//}
//
//// Close the file.
//error = fclose(filePtr);
//if (error != 0)
//{
//	return false;
//}
//
//// Initialize the position in the image data buffer.
//k = 0;
//
//// Read the image data into the height map array.
//for (j = 0; j<m_terrainHeight; j++)
//{
//	for (i = 0; i<m_terrainWidth; i++)
//	{
//		// Bitmaps are upside down so load bottom to top into the height map array.
//		index = (m_terrainWidth * (m_terrainHeight - 1 - j)) + i;
//
//		// Get the grey scale pixel value from the bitmap image data at this location.
//		height = bitmapImage[k];
//
//		// Store the pixel value as the height at this point in the height map array.
//		m_heightMap[index].y = (float)height;
//
//		// Increment the bitmap image data index.
//		k += 3;
//	}
//
//	// Compensate for the extra byte at end of each line in non-divide by 2 bitmaps (eg. 257x257).
//	k++;
//}
//
//// Release the bitmap image data now that the height map array has been loaded.
//delete[] bitmapImage;
//bitmapImage = 0;
//
//// Release the terrain filename now that is has been read in.
//delete[] m_terrainFilename;
//m_terrainFilename = 0;
//
//return true;