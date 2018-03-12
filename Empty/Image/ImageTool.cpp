#include <PCH.h>
#include <Image/ImageTool.h>
#include <Graphics/GL4/GLTexture.h>
#include <Graphics/GL4/GLConfig.h>

void ImageTool::LoadDDSFromFileToGL(const std::string& filename, GLTexture* texture)
{
	//https://www.opengl.org/discussion_boards/archive/index.php/t-126118.html

	FILE* file;

	file = fopen(filename.c_str(), "rb");

	if (!file) LOG_ERROR("failed to find file");

	char fileCode[4];
	fread(fileCode, 1, 4, file);

	if (strncmp(fileCode, "DDS ", 4) != 0) LOG_ERROR("please dds file");

	unsigned char proxy[124];
	fread(&proxy, 124, 1, file);
	
	//-------------- read from headers from file ---------------
	GLDDS dds;
	dds.height = *(unsigned int*)&(proxy[8]);
	dds.width = *(unsigned int*)&(proxy[12]);
	dds.linearSize = *(unsigned int*)&(proxy[16]);
	dds.mipmapNum = *(unsigned int*)&(proxy[24]);
	uint selectType = *(unsigned int*)&(proxy[80]);

	//-------------------- create buffer -----------------------
	dds.bufferSize = (dds.mipmapNum > 1) ? dds.linearSize * 2 : dds.linearSize;
	dds.buffer = (unsigned char*)malloc(dds.bufferSize * sizeof(unsigned char));
	fread(dds.buffer, 1, dds.bufferSize, file);

	//TODO : RGA is not support 
	switch (selectType)
	{
	case FOURCC_DXT1:
	{
		dds.format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	}
	case FOURCC_DXT3:
	{
		dds.format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	}
	case FOURCC_DXT5:
	{
		dds.format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	}
	default:
		dds.format = UINT_MAX;
		assert(0 && "fatal : failed to find dds type");
		break;
	}

	//create gl textures
	glGenTextures(1, &texture->mTexture);
	glBindTexture(GL_TEXTURE_2D, texture->mTexture);

	//using slice memory alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//find width memory slice block
	uint blockSize = dds.format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16;
	uint offset = 0;

	uint width = dds.width;
	uint height = dds.height;
	for (uint level = 0; level < dds.mipmapNum; ++level)
	{
		uint size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

		//we dont allow to 0 x 0 texture
		if (width == 0 && height == 0) continue;		//break when 0

		auto currentBuffer = (unsigned char*)dds.buffer + offset;
		glCompressedTexImage2DARB(GL_TEXTURE_2D,
			level,
			dds.format,
			width, height, 0, size, currentBuffer);

		offset += size;
		width /= 2;
		height /= 2;
	}

	texture->width = dds.width;
	texture->height = dds.height;

	free(dds.buffer);
	fclose(file);
}
