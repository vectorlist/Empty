#pragma once

//TODO : SKyCube SkyDome

//TODO 1 : Init Texture From images

typedef std::vector<std::string> ImagesArrays;
 
class Environment
{
public:
	Environment() = default;

	virtual void Init() = 0;
	//Sky Box Only
	virtual bool InitFromImages(const std::vector<std::string>& images) { return false; };

	virtual void Render(/*const float* project, const float* view*/) = 0;

};

//Test
#define CUBE_FACE_FT		0
#define CUBE_FACE_BK		1
#define CUBE_FACE_LF		2
#define CUBE_FACE_RT		3
#define CUBE_FACE_UP		4
#define CUBE_FACE_DN		5
#define CUBE_FACE_MAX		6
#include <Graphics/DX11/DXConfig.h>

class SkyCube : public Environment
{
public:
	SkyCube();
	virtual~SkyCube();

	void Init();
	bool InitFromImages(const std::vector<std::string>& images);

	void Render();

	Shader*							mShader;
	ID3D11SamplerState*				mSampler;

	std::array<Texture*, 6>			mTextures;
	std::array<VertexBuffer*, 6>	mVerticesBuffers;
	IndexBuffer*					mIndexBuffer;
	
	ID3D11DepthStencilState* mDepthStencilState = nullptr;
	ID3D11SamplerState* mSamplerState = nullptr;
};

//desc.DepthWriteMask maps to glDepthMask value
//desc.DepthEnable = true / false corresponds to glEnable(GL_DEPTH_TEST) / glDisable(GL_DEPTH_TEST) respectively
//desc.DepthFunc maps to glDepthFunc value
//only turn on stenciling and assign the rest of the fields if you're using stenciling.
//shareimprove this answer

//void glStencilFunc(GLenum func,
//	GLint ref,
//	GLuint mask);

//#define GL_NEVER                          0x0200
//#define GL_LESS                           0x0201
//#define GL_EQUAL                          0x0202
//#define GL_LEQUAL                         0x0203
//#define GL_GREATER                        0x0204
//#define GL_NOTEQUAL                       0x0205
//#define GL_GEQUAL                         0x0206
//#define GL_ALWAYS                         0x0207


//Sky Texture Image To Texture Menualy

//CUBE ORDER

//FRONT > BACK > LEFT > RIGHT > TOP > BACK

//             ( 1, 1, 1)
//    5 -------- 6
//   /          /
//  1 -------- 2 |
//  |          | |
//  | 4        | 7
//  |/         |/
//  0 -------- 3
//(-1,-1, -1)

// FRONT
// (-1, 1, 1) (1, 1, 1)
//  0 -------- 1 
//  |          | 
//  |          | 
//  |          |
//  3 -------- 2
//(-1,-1, 1)  (1, -1, 1)
// Order Triangle Strip 0 1 3 2