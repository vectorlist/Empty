#include <Core/PCH.h>
#include <Graphics/TypeTransform.h>
#include <Graphics/DX11/DXConfig.h>
#include <Graphics/GL4/GLConfig.h>

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
GLenum GetTopology(Topology topology)
{
	switch (topology)
	{
	case Topology::POINTS:
		return GL_POINTS;
	case Topology::LINES:
		return GL_LINES;
	case Topology::LINE_STRIP:
		return GL_LINE_STRIP;
	case Topology::TRIANGLES:
		return GL_TRIANGLES;
	case Topology::TRIANGLE_STRIP:
		return GL_TRIANGLE_STRIP;
	default:
		return GL_NONE;
	}
}
GLenum GetInternalFormat(Format format)
{
	switch (format)
	{
	case Format::FORMAT_RGBA32_UINT:
		return GL_RGBA32UI;
	case Format::FORMAT_RGBA32_FLOAT:
		return GL_RGBA32F;
	case Format::FORMAT_RGB32_UINT:
		return GL_RGB32UI;
	case Format::FORMAT_RGB32_FLOAT:
		return GL_RGB32F;
	case Format::FORMAT_R32_UINT:
		return GL_R32UI;
	case Format::FORMAT_R32_FLOAT:
		return GL_R32F;
	case Format::FORMAT_RGBA16_UNIT:
		return GL_RGBA16UI;
	case Format::FORMAT_RGBA16_FLOAT:
		return GL_RGBA16F;
	case Format::FORMAT_RGB16_UINT:
		return GL_RGB16UI;
	case Format::FORMAT_RGB16_FLOAT:
		return GL_RGB16F;
	case Format::FORMAT_R16_UINT:
		return GL_R16UI;
	case Format::FORMAT_R16_FLOAT:
		return GL_R16F;
	case Format::FORMAT_RGBA8_UINT:
		return GL_RGBA8UI;
	case Format::FORMAT_RGBA8_UNORM:
		return GL_RGBA8;
	case Format::FORMAT_RGB8_UINT:
		return GL_RGB8UI;
	case Format::FORMAT_RGB8_UNORM:
		return GL_RGB8;
	case Format::FORMAT_R8_UINT:
		return GL_R8UI;
	case Format::FORMAT_R8_UNORM:
		return GL_R8;
	default:
		return GL_NONE;
	}
}
GLenum GetFormat(Format format)
{
	switch (format)
	{
	case Format::FORMAT_RGBA32_UINT:
		return GL_RGBA;
	case Format::FORMAT_RGBA32_FLOAT:
		return GL_RGBA;
	case Format::FORMAT_RGB32_UINT:
		return GL_RGB;
	case Format::FORMAT_RGB32_FLOAT:
		return GL_RGB;
	case Format::FORMAT_R32_UINT:
		return GL_RED;
	case Format::FORMAT_R32_FLOAT:
		return GL_RED;
	case Format::FORMAT_RGBA16_UNIT:
		return GL_RGBA;
	case Format::FORMAT_RGBA16_FLOAT:
		return GL_RGBA;
	case Format::FORMAT_RGB16_UINT:
		return GL_RGB;
	case Format::FORMAT_RGB16_FLOAT:
		return GL_RGB;
	case Format::FORMAT_R16_UINT:
		return GL_RED;
	case Format::FORMAT_R16_FLOAT:
		return GL_RED;
	case Format::FORMAT_RGBA8_UINT:
		return GL_RGBA;
	case Format::FORMAT_RGBA8_UNORM:
		return GL_RGBA;
	case Format::FORMAT_RGB8_UINT:
		return GL_RGB;
	case Format::FORMAT_RGB8_UNORM:
		return GL_RGB;
	case Format::FORMAT_R8_UINT:
		return GL_RED;
	case Format::FORMAT_R8_UNORM:
		return GL_RED;
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

D3D_PRIMITIVE_TOPOLOGY GetTopology(Topology topology)
{
	switch (topology)
	{
	case Topology::POINTS:
		return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
	case Topology::LINES:
		return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	case Topology::LINE_STRIP:
		return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
	case Topology::TRIANGLES:
		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case Topology::TRIANGLE_STRIP:
		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	default:
		return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}
}
DXGI_FORMAT GetFormat(Format format)
{
	switch (format)
	{
	case Format::FORMAT_RGBA32_UINT:
		return DXGI_FORMAT_R32G32B32A32_UINT;
	case Format::FORMAT_RGBA32_FLOAT:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case Format::FORMAT_RGB32_UINT:
		return DXGI_FORMAT_R32G32B32_UINT;
	case Format::FORMAT_RGB32_FLOAT:
		return DXGI_FORMAT_R32G32B32_FLOAT;
	case Format::FORMAT_R32_UINT:
		return DXGI_FORMAT_R32_UINT;
	case Format::FORMAT_R32_FLOAT:			
		return DXGI_FORMAT_R32_FLOAT;
	case Format::FORMAT_RGBA16_UNIT:
		return DXGI_FORMAT_R16G16B16A16_UINT;
	case Format::FORMAT_RGBA16_FLOAT:
		return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case Format::FORMAT_RGB16_UINT:				//invalid 48bit format 
		return DXGI_FORMAT_UNKNOWN;
	case Format::FORMAT_RGB16_FLOAT:
		return DXGI_FORMAT_UNKNOWN;
	case Format::FORMAT_R16_UINT:
		return DXGI_FORMAT_R16_UINT;
	case Format::FORMAT_R16_FLOAT:
		return DXGI_FORMAT_R16_FLOAT;
	case Format::FORMAT_RGBA8_UINT:
		return DXGI_FORMAT_R8G8B8A8_UINT;		//RGB8 dosent support 24bit
	case Format::FORMAT_RGBA8_UNORM:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case Format::FORMAT_RGB8_UINT:
		return DXGI_FORMAT_R8G8_UINT;
	case Format::FORMAT_RGB8_UNORM:
		return DXGI_FORMAT_R8G8_UNORM;
	case Format::FORMAT_R8_UINT:
		return DXGI_FORMAT_R8_UINT;
	case Format::FORMAT_R8_UNORM:
		return DXGI_FORMAT_R8_UNORM;
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}
}

