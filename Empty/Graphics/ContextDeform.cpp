#include <PCH.h>
#include "Graphics/ContextDeform.h"
#include <Graphics/DX11/DXConfig.h>
#include <Graphics/GL4/GLConfig.h>

namespace deform
{
	DXGI_FORMAT GetDXFormat(Format format)
	{
		switch (format)
		{
		case Format::FORMAT_R32G32B32A32_UINT:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		case Format::FORMAT_R32G32B32_UINT:
			return DXGI_FORMAT_R32G32B32_UINT;
		case Format::FORMAT_R16G16B16A16_UINT:
			return DXGI_FORMAT_R16G16B16A16_UINT;
		case Format::FORMAT_R16G16_UINT:
			return DXGI_FORMAT_R16G16_UINT;
		case Format::FORMAT_R8G8B8A8_UINT:
			return DXGI_FORMAT_R8G8B8A8_UINT;
		case Format::FORMAT_R8G8_UINT:
			return DXGI_FORMAT_R8G8_UINT;
		case Format::FORMAT_R8_UNORM:
			//A single-component, 8-bit unsigned-normalized-integer format that supports 8 bits for the red channel.
			return DXGI_FORMAT_R8_UNORM;
		case Format::FORMAT_R8_UINT:					//char
			return DXGI_FORMAT_R8_UINT;
		case Format::FORMAT_UNKNOWN:
			return DXGI_FORMAT_UNKNOWN;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}
	uint GetGLFormatDemention(Format format)
	{
		switch (format)
		{
		case Format::FORMAT_R32G32B32A32_UINT:
			return GL_RGBA32UI;
		case Format::FORMAT_R8G8B8A8_UINT:
			return GL_RGBA8_SNORM;
		case Format::FORMAT_R8_UNORM:
			return GL_RED;
		case Format::FORMAT_R8_UINT:
			return GL_RED;
		case Format::FORMAT_UNKNOWN:
			return 0;
		default:
			return 0;
		}
	}
	uint GetGLFormat(Format format)
	{
		switch (format)
		{
		case Format::FORMAT_R8_UNORM:
			return GL_UNSIGNED_BYTE;
		case Format::FORMAT_R8_UINT:	
			return GL_UNSIGNED_INT;
		case Format::FORMAT_UNKNOWN:
			return 0;
		default:
			return 0;
		}
	}
}


