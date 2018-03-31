#include <Core/PCH.h>
#include <Graphics/GL4/GLTexture.h>
#include <Graphics/GL4/GLConfig.h>
#include <Image/ImageTool.h>

#include <Graphics/TypeTransform.h>

#include <Image/Image.h>

GLTexture::GLTexture()
	: mTexture(0)
{
}

GLTexture::~GLTexture()
{
	if (mTexture) glDeleteTextures(1, &mTexture);
}

void GLTexture::InitFromDDS(const std::string& filename)
{
	ImageTool::LoadDDSFromFileToGL(filename, this);
}

void GLTexture::InitFromImage(const std::string& filename)
{
	Image image;
	image.LoadFromFile(filename.c_str(), IMAGE_REQ_RGBA);		//request 4 componnent

	//check graphical mipmap capbillity
	width = image.GetWidth();
	height = image.GetHeight();

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	glTextureParameteri(mTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(mTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(mTexture, GL_TEXTURE_COMPARE_FUNC, GL_NEVER);
	glTextureParameteri(mTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(mTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//test
	switch (image.GetFormat())
	{
	case PixelFormat::R8:
	{
		//TODO : unpack aligned
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image.GetWidth(), image.GetHeight(), 0,
			GL_RED, GL_UNSIGNED_BYTE, (GLvoid*)image.GetData());
		break;
	}
	case PixelFormat::RGB8:
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.GetWidth(), image.GetHeight(), 0,
			GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)image.GetData());
		break;
	}
	case PixelFormat::RGBA8:
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.GetWidth(), image.GetHeight(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image.GetData());
		break;
	}
	default:
		ASSERT_MSG(0, "Invalid Image Format");
		break;
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::Init(TextureCreateInfo& info)
{
	//this->width = info.width;
	//this->height = info.height;
	//
	////glCreateTextures(GL_TEXTURE_2D, 1, &mTexture);		//auto bind
	//glGenTextures(1, &mTexture);
	//glBindTexture(GL_TEXTURE_2D, mTexture);
	//
	////must be red 0x1903
	//uint format = deform::GetGLFormatDemention(info.format);
	//bool matched = false;
	//if (format == 0x1903) {
	//	matched = true;
	//}

	////must be byte 0x1401
	//uint type = deform::GetGLFormat(info.format);

	//if (type == 0x1401) {
	//	matched = true;
	//}
	//
	//glTexImage2D(GL_TEXTURE_2D, 0, format, info.width, info.height, 0,
	//	format, type, info.pData);

	////DSA
	////glTextureStorage2D(mTexture, 0, GL_RED, info.width, info.height);
	////set alglined
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	////sampler
	//glTextureParameteri(mTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTextureParameteri(mTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTextureParameteri(mTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	////glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTextureParameteri(mTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	////fill pixels
	////glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, info.pData);

	//glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::Bind(uint index)
{
	glBindTextureUnit(index, mTexture);
}
