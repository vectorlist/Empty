#pragma once

#include <Graphics/GL4/glshader.h>
#include <Graphics/dx11/dxshader.h>

//Internal Shaders
namespace glsl
{
extern const char* sceneVS;
extern const char* sceneFS;

extern const char* aabbVS;
extern const char* aabbFS;

extern const char* instanceFontVS;
extern const char* instanceFontFS;
}

class GLInternalShader : public GLShader
{
public:
	GLInternalShader() {};
};

namespace hlsl
{
extern const char* sceneVS;
extern const char* sceneFS;

extern const char* aabbVS;
extern const char* aabbFS;

extern const char* instanceFontVS;
extern const char* instanceFontFS;
}

class DXInternalSceneShader : public DXShader
{
public:
	DXInternalSceneShader(){}

	void CreateInputLayout(ID3DBlob* vBlob) override;
};

class DXInternalAABBShader : public DXShader
{
public:
	DXInternalAABBShader(){}
	void CreateInputLayout(ID3DBlob* vBlob) override;
};

//Instancing Font Vertex Input layout
//struct FontInstanceVertex
//{
//	vec4f pwh;				//pos width height 16
//	vec4f uv;				//uv1   uv2        32
//	vec4f color;			//rgba             48
//};

class DXInternalFontShader : public DXShader
{
public:
	DXInternalFontShader() {};
	void CreateInputLayout(ID3DBlob* vBlob) override;
};

//class DXInternalInstanceFontShader : public 

//             ( 1, 1, 1)
//    5 -------- 6
//   /          /
//  1 -------- 2 |
//  |          | |
//  | 4        | 7
//  |/         |/
//  0 -------- 3
//(-1,-1, -1)