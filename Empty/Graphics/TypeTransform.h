#pragma once

#include <core/type.h>

enum DXGI_FORMAT;
enum D3D_PRIMITIVE_TOPOLOGY;

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef int GLsizei;
typedef unsigned int GLuint;

namespace GLTransform
{
GLenum GetVertextFormat(VertexFormat format);
GLenum GetTopology(Topology topology);
GLenum GetInternalFormat(Format format);
GLenum GetFormat(Format format);
}

namespace DXTransform
{
DXGI_FORMAT				GetVertexFormat(VertexFormat format, uint formatSize);
D3D_PRIMITIVE_TOPOLOGY	GetTopology(Topology topology);
DXGI_FORMAT GetFormat(Format format);
}

//#define GL_RED 0x1903
//#define GL_GREEN 0x1904
//#define GL_BLUE 0x1905
//#define GL_ALPHA 0x1906
//#define GL_RG8UI 0x8238
//#define GL_RG16UI 0x823A
//#define GL_RGBA8UI 0x8D7C

//#define GL_BYTE 0x1400
//#define GL_UNSIGNED_BYTE 0x1401
//#define GL_SHORT 0x1402
//#define GL_UNSIGNED_SHORT 0x1403
//#define GL_INT 0x1404
//#define GL_UNSIGNED_INT 0x1405
//#define GL_FLOAT 0x1406