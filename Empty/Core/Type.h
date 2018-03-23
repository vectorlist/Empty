#pragma once

#include <math/vec2f.h>
#include <math/vec3f.h>
#include <math/matrix4x4.h>

typedef unsigned int	uint;
typedef unsigned char	uint8;
typedef unsigned int	Enum;

//-------------------------- Buffer Type ---------------------------

struct VertexPS   // position : st
{
	vec3f pos;
	vec2f st;
};

struct VertexPSC
{
	vec3f pos;
	vec2f st;
	vec3f color;
};

struct VertexPNSBT
{
	vec3f pos;
	vec3f normal;
	vec2f st;
	vec3f binormal;
	vec3f tangent;
};

struct SpriteVertex
{
	vec3f pos;
	vec2f st;
};

struct FontVertex
{
	vec2f pos;
	vec2f st;
};

struct Vertex
{
	vec3f pos;
	vec3f normal;
	vec2f st;
	vec3f binormal;
	vec3f tangent;
};

struct DebugVertex
{
	vec3f pos;
	vec3f color;
};

/////////////////// CBO or UBO /////////////////////////////
struct Matrices					//default matrice
{
	Matrix4x4 proj;
	Matrix4x4 view;
	Matrix4x4 model;
};

struct UboMatrice
{
	Matrix4x4 proj;
	Matrix4x4 view;
};

//--------------------------- etc type -----------------------------------
enum class GraphicAPI : uint
{
	DIRECTX11,
	OPENGL45,
	VULKAN
};

enum ApplicationFlagBit
{
	APP_NONE		= 1 << 0,
	APP_CENTERPOS	= 1 << 1,
	APP_TITLEBAR	= 1 << 2
};

typedef uint ApplicationFlag;

//Core Type

struct Viewport
{
	uint x, y, w, h;
};

//========================= Context Type ================================

enum class Format
{
	//TODO : Add more formats
	FORMAT_UNKNOWN = 0,
	FORMAT_R32G32B32A32_UINT,
	FORMAT_R32G32B32_UINT,
	FORMAT_R16G16B16A16_UINT,
	FORMAT_R16G16_UINT,
	FORMAT_R8G8B8A8_UINT,
	FORMAT_R8G8_UINT,
	FORMAT_R8_UNORM,
	FORMAT_R8_UINT,
};

//TODO : make Device CPP

struct TextureCreateInfo
{
	void* pData;
	uint width;
	uint height;
	Format format;
	uint mipLevels;
	//TODO Bind Type (Render traget) to shader texture or context renderer
};

enum class BufferType
{
	BUFFER_DEFAULT,
	BUFFER_STATIC,
	BUFFER_DYNAMIC			
};

enum class BufferBinding
{
	BIND_VERTEX,
	BIND_INDEX,
	BIND_UNIFORM
};

struct BufferCreateInfo
{
	void*			pData;
	uint 			size;
	BufferType		type;
	BufferBinding	binding;
};

//TODO REMOVE
enum class InputType : uint
{
	BYTE = 0xFF,
	INT,
	UINT,
	SHORT,
	FLOAT,
	DOUBLE
};

//TODO REMOVE
struct InputLayout
{
	uint slot;
	InputType elementType;
	uint elemintDemension;		//how many compponent type 
	uint stride;
	uint offset;
};

enum class Topology : Enum
{
	POINT = 0xF0F0,
	LINE,
	LINE_STRIP,
	TRIANGLE,
	TRIANGLE_STRIP,
	QUAD,
	UNKWON
};

enum class VertexFormat : Enum
{
	BYTE = 0xFF,
	INT,
	UINT,
	SHORT,
	FLOAT,
	DOUBLE
};

//TODO : slot index
typedef struct VertexAttrib
{
	uint			Location;
	VertexFormat	Format;
	uint			FormatSize;
	uint			Stride;
	uint			Offset;
}VertexInputAttrib;
//=================== Vertex Buffer ===================

struct VertexBufferCreateInfo
{
	BufferType			Type;
	void*				pVertices;
	uint				VerticesSize;
	VertexAttrib*		pAttrib;
	uint				AttribSize;
};

//================== Index Buffer =====================

struct IndexBufferCreateInfo
{
	void*				pIndices;
	uint				IndicesSize;
};

//====================== Model ========================

struct ModelCreateInfo
{
	void*				pVertices;
	uint				VerticesSize;
	void*				pIndices;
	uint				IndicesSize;
	VertexAttrib*		pAttrib;
	uint				AttribSize;
};

//==================== Node ===========================

typedef struct TerrainNodeVertex
{
	vec3f pos;
	vec3f normal;
}NodeVertex;

//=================== TERRAIN =========================
struct TerrainCreateInfo
{
	const char* Filename;
	float		CellScale;
	float		HeightScale;
	uint		nTiles;
	uint		nSmooth;
};

struct TerrainVertex
{
	vec3f pos;
	vec3f normal;
	vec2f st;
	vec2f uv;
	vec3f binormal;
	vec3f tangent;
};

struct TriangleIndex
{
	uint p0;
	uint p1;
	uint p2;
};

//================ RASTER STATE ====================


//================ DEPTH STATE =====================

enum class DepthMask : uint 
{ 
	NONE = 0x00,
	ALL = 0xFF
};
enum class DepthFunc : uint
{
	LESS = 0U,
	EQUAL,
	LESS_EQUAL,
	GREATER,
	NOT_EQUAL,
	GREATER_EQUAL,
	ALWAYS
};

struct DepthStencilState
{
	DepthMask	Mask;			//Stencil Mask
	bool		Enabled;		//enable stencil
	DepthFunc   Func;			//Comparison fun
};

//============== SAMPLER STATE ===================



struct SamplerCreateInfo
{

};
