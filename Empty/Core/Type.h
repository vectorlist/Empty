#pragma once

#include <math/vec2f.h>
#include <math/vec3f.h>
#include <math/matrix4x4.h>

typedef unsigned int	uint;
typedef unsigned char	uint8;

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

struct Data0
{
	vec3f viewPos;		//12	//must be pack of 16bytes
	float intensity;	//4
	vec3f lightPos;		//12
	float padding;		//4
};//total 32


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
	BufferBinding	binding;			//dont use at uniform buffer
};

