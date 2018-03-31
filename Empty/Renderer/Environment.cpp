#include <Core/PCH.h>
#include "Environment.h"

#include <Cache/BufferCache.h>
#include <Cache/ShaderCache.h>

#include <Cache/TextureCache.h>
#include <Image/Image.h>

#include <Graphics/Context.h>
#include <Graphics/GL4/GLConfig.h>
//Directx11 Only
//#include <Graphics/DX11/DXConfig.h>


SkyCube::SkyCube()
{
	Init();
}

SkyCube::~SkyCube()
{
}

void SkyCube::Init()
{
	//TODO Draw order Striangle Strib
	std::vector<std::vector<VertexPS>> faces(CUBE_FACE_MAX);

	faces[0] = {
		{ vec3f(-1.0, 1.0f, 1.0f), vec2f( 0.0f, 0.0f) },
		{ vec3f( 1.0, 1.0f, 1.0f), vec2f( 1.0f, 0.0f) },
		{ vec3f( 1.0,-1.0f, 1.0f), vec2f( 1.0f, 1.0f) },
		{ vec3f(-1.0,-1.0f, 1.0f), vec2f( 0.0f, 1.0f) }
	};

	faces[1] = {
		{ vec3f( 1.0, 1.0f,-1.0f), vec2f(0.0f, 0.0f) },
		{ vec3f(-1.0, 1.0f,-1.0f), vec2f(1.0f, 0.0f) },
		{ vec3f(-1.0,-1.0f,-1.0f), vec2f(1.0f, 1.0f) },
		{ vec3f( 1.0,-1.0f,-1.0f), vec2f(0.0f, 1.0f) }
	};

	faces[2] = {
		{ vec3f(-1.0, 1.0f,-1.0f), vec2f(0.0f, 0.0f) },
		{ vec3f(-1.0, 1.0f, 1.0f), vec2f(1.0f, 0.0f) },
		{ vec3f(-1.0,-1.0f, 1.0f), vec2f(1.0f, 1.0f) },
		{ vec3f(-1.0,-1.0f,-1.0f), vec2f(0.0f, 1.0f) }
	};

	faces[3] = {
		{ vec3f( 1.0, 1.0f, 1.0f), vec2f(0.0f, 0.0f) },
		{ vec3f( 1.0, 1.0f,-1.0f), vec2f(1.0f, 0.0f) },
		{ vec3f( 1.0,-1.0f,-1.0f), vec2f(1.0f, 1.0f) },
		{ vec3f( 1.0,-1.0f, 1.0f), vec2f(0.0f, 1.0f) }
	};

	//TOP
	faces[4] = {
		{ vec3f(-1.0, 1.0f,-1.0f), vec2f(1.0f, 1.0f) },
		{ vec3f( 1.0, 1.0f,-1.0f), vec2f(0.0f, 1.0f) },
		{ vec3f( 1.0, 1.0f, 1.0f), vec2f(0.0f, 0.0f) },
		{ vec3f(-1.0, 1.0f, 1.0f), vec2f(1.0f, 0.0f) }
	};

	//BOTTOM
	faces[5] = {
		{ vec3f(-1.0,-1.0f, 1.0f), vec2f(1.0f, 0.0f) },
		{ vec3f( 1.0,-1.0f, 1.0f), vec2f(1.0f, 1.0f) },
		{ vec3f( 1.0,-1.0f,-1.0f), vec2f(0.0f, 1.0f) },
		{ vec3f(-1.0,-1.0f,-1.0f), vec2f(0.0f, 0.0f) }
	};

	std::vector<uint> indices = {0,1,3,2};

	std::vector<VertexAttrib> attribs = {
		//Location / Format / Format Count / Stride / Offset
		{ 0, VertexFormat::FLOAT, 3, sizeof(VertexPS), offsetof(VertexPS, VertexPS::pos)},
		{ 0, VertexFormat::FLOAT, 2, sizeof(VertexPS), offsetof(VertexPS, VertexPS::st) }
	};

	for (int i = 0; i < faces.size(); ++i)
	{
		VertexBufferCreateInfo vertexInfo{};
		vertexInfo.pVertices = faces[i].data();
		vertexInfo.VerticesSize = faces[i].size() * sizeof(VertexPS);
		vertexInfo.pAttrib = attribs.data();
		vertexInfo.AttribSize = attribs.size();

		mVerticesBuffers[i] = BufferCache::CreateVertexBuffer(&vertexInfo);
	}

	IndexBufferCreateInfo indexInfo{};
	indexInfo.pIndices = indices.data();
	indexInfo.IndicesSize = indices.size() * sizeof(uint);
	
	mIndexBuffer = BufferCache::CreateIndexBuffer(&indexInfo);

	mShader = ShaderCache::CreateShader("EnvironmentVS", "EnvironmentPS");

	//================= TEXTURE ====================
	std::vector<std::string> cubemap = {
		"../data/texture/clearsky/FT.jpg",
		"../data/texture/clearsky/BK.jpg",
		"../data/texture/clearsky/RT.jpg",
		"../data/texture/clearsky/LF.jpg",
		"../data/texture/clearsky/UP.jpg",
		"../data/texture/clearsky/DN.jpg"
	};

	//std::vector<std::string> cubemap = {
	//	"../data/texture/hazesky/FT.jpg",
	//	"../data/texture/hazesky/BK.jpg",
	//	"../data/texture/hazesky/RT.jpg",
	//	"../data/texture/hazesky/LF.jpg",
	//	"../data/texture/hazesky/UP.jpg",
	//	"../data/texture/hazesky/DN.jpg"
	//};

	this->InitFromImages(cubemap);

	if (GContext->GetApiType() == GraphicAPI::DIRECTX11) {
		D3D11_SAMPLER_DESC d3dSamplerDesc;
		ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
		d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		d3dSamplerDesc.MinLOD = 0;
		d3dSamplerDesc.MaxLOD = 0;
		LOG_HR << G_DXDevice->CreateSamplerState(&d3dSamplerDesc, &mSamplerState);
	}
}

bool SkyCube::InitFromImages(const std::vector<std::string>& images)
{
	//More Conditional
	for (int i = 0; i < images.size(); ++i) {
		mTextures[i] = TextureCache::LoadTexture(images[i].c_str());
	}
	
	//Create Sampler
	


	return true;
}

void SkyCube::Render()
{
	DepthStencilState state;
	state.Enabled = true;
	state.Mask = DepthMask::ALL;
	state.Func = DepthFunc::LESS_EQUAL;

	GContext->SetDepthStencilEx(&state);
	
	mShader->Bind();

	for (int i = 0; i < mVerticesBuffers.size(); ++i) {
		mVerticesBuffers[i]->Bind();
		mTextures[i]->Bind(0);
		
		if (mSamplerState) {
			G_DXContext->PSSetSamplers(0, 1, &mSamplerState);
		}
		mIndexBuffer->Bind();
		mIndexBuffer->DrawIndexed(Topology::TRIANGLE_STRIP, 4);
	}
	
	state.Func = DepthFunc::LESS;
	GContext->SetDepthStencilEx(&state);
}
