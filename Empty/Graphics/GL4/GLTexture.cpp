#include <PCH.h>
#include <Graphics/GL4/GLTexture.h>
#include <Graphics/GL4/GLConfig.h>
#include <Image/Image.h>

#include <Graphics/ContextDeform.h>

GLTexture::GLTexture()
	: mTexture(0)
{
}

GLTexture::~GLTexture()
{

	if (mTexture) glDeleteTextures(1, &mTexture);
}

void GLTexture::InitFromFile(const std::string& filename)
{
	Image::LoadDDSFromFileToGL(filename, this);
}

void GLTexture::Init(TextureCreateInfo& info)
{
	this->width = info.width;
	this->height = info.height;
	
	//glCreateTextures(GL_TEXTURE_2D, 1, &mTexture);		//auto bind
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	
	//must be red 0x1903
	uint format = deform::GetGLFormatDemention(info.format);
	bool matched = false;
	if (format == 0x1903) {
		matched = true;
	}

	//must be byte 0x1401
	uint type = deform::GetGLFormat(info.format);

	if (type == 0x1401) {
		matched = true;
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, format, info.width, info.height, 0,
		format, type, info.pData);

	//DSA
	//glTextureStorage2D(mTexture, 0, GL_RED, info.width, info.height);
	//set alglined
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//sampler
	glTextureParameteri(mTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(mTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(mTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(mTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//fill pixels
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, info.pData);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::Bind(uint index)
{
	glBindTextureUnit(index, mTexture);
}
