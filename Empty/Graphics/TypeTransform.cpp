#include <PCH.h>
#include <Graphics/TypeTransform.h>
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

namespace GLTransform
{
GLenum GetVertextFormat(VertexFormat format)
{
	switch (format)
	{
	case VertexFormat::BYTE:
		return GL_BYTE;
	case VertexFormat::INT:
		return GL_INT;
	case VertexFormat::UINT:
		return GL_UNSIGNED_INT;
	case VertexFormat::SHORT:
		return GL_SHORT;
	case VertexFormat::FLOAT:
		return GL_FLOAT;
	case VertexFormat::DOUBLE:
		return GL_DOUBLE;
	default:
		return GL_NONE;
	}
}
}

namespace DXTransform
{
DXGI_FORMAT GetVertexFormat(VertexFormat format, uint formatSize)
{
	switch (formatSize)
	{
	case 1:
	{
		switch (format)
		{
		case VertexFormat::BYTE:		
			return DXGI_FORMAT_R8_UNORM;
		case VertexFormat::FLOAT:		
			return DXGI_FORMAT_R32_FLOAT;
		case VertexFormat::INT:
			return DXGI_FORMAT_R32_SINT;
		case VertexFormat::UINT:
			return DXGI_FORMAT_R32_UINT;
		default:
			ASSERT_MSG(0, "Invalid type");
			break;
		}
		break;
	}
	case 2:
	{
		switch (format)
		{
		case VertexFormat::BYTE:
			return DXGI_FORMAT_R8G8_UNORM;
		case VertexFormat::FLOAT:
			return DXGI_FORMAT_R32G32_FLOAT;
		case VertexFormat::INT:
			return DXGI_FORMAT_R32G32_SINT;
		case VertexFormat::UINT:
			return DXGI_FORMAT_R32G32_UINT;
		default:
			ASSERT_MSG(0, "Invalid type");
			break;
		}
		break;
	}
	case 3:
	{
		switch (format)
		{
		case VertexFormat::BYTE:
			ASSERT_MSG(0, "DirectX dosent support 24 bits");
			break;
		case VertexFormat::FLOAT:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case VertexFormat::INT:
			return DXGI_FORMAT_R32G32B32_SINT;
		case VertexFormat::UINT:
			return DXGI_FORMAT_R32G32B32_UINT;
		default:
			ASSERT_MSG(0, "Invalid type");
			break;
		}
		break;
	}
	case 4:
	{
		switch (format)
		{
		case VertexFormat::BYTE:
			ASSERT_MSG(0, "DirectX dosent support 24 bits");
			break;
		case VertexFormat::FLOAT:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case VertexFormat::INT:
			return DXGI_FORMAT_R32G32B32A32_SINT;
		case VertexFormat::UINT:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		default:
			ASSERT_MSG(0, "Invalid type");
			break;
		}
		break;
	}
	default:
		ASSERT_MSG(0, "Invalid type");
		break;
	}
}
}

