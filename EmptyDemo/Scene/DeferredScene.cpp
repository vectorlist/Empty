#include <Core/PCH.h>
#include <Scene/DeferredScene.h>

#include <Core/Camera.h>
#include <Core/Input.h>
#include <Graphics/Context.h>

#include <Cache/BufferCache.h>

#include <Renderer/Batch/FontBatch.h>
#include <Renderer/Batch/SpriteBatch.h>

#include <Renderer/Environment.h>

#include <Cache/ModelCache.h>
#include <Cache/ShaderCache.h>
#include <Cache/TextureCache.h>
#include <Cache/TerrainCache.h>

#include <Core/Profiler.h>
#include <Renderer/Batch/DebugBatch.h>

#include <Math/Random.h>
#include <Renderer/Environment.h>

//Test
#include <Graphics/GL4/GLConfig.h>
#include <Graphics/GL4/GLFrameBuffer.h>
#include <Core/Profiler.h>

DeferredScene::DeferredScene()
	: mFrameBuffer(nullptr)
{
}

DeferredScene::~DeferredScene()
{
	SAFE_DELETE(mFrameBuffer);
}

void DeferredScene::InitScene()
{
	Viewport v;
	GContext->GetViewport(v);

	mCamera = CreateSceneCamera("Main Camera", 45.f, (float)v.w / (float)v.h, 0.1f, 1000.f);

	BufferCreateInfo info{};
	info.pData = nullptr;
	info.size = sizeof(Matrix4x4) * 2;
	info.type = BufferType::BUFFER_DYNAMIC;
	info.binding = BufferBinding::BIND_UNIFORM;

	mUbo0 = BufferCache::CreateUniformBuffer(&info);
	info.size = sizeof(Matrix4x4);
	mUbo1 = BufferCache::CreateUniformBuffer(&info);

	info.size = sizeof(vec4f);
	mUbo2 = BufferCache::CreateUniformBuffer(&info);

	/*info.size = sizeof(vec3f) * 2 * 10;
	mUbo3 = BufferCache::CreateUniformBuffer(&info);*/

	mEnviron = new SkyCube;

	mModel = ModelCache::LoadModel("../data/model/knot.obj");
	
	mModel->mNodes[0].texture = TextureCache::LoadTexture("../data/texture/concrete.dds");
	//mModel->mNodes[1].texture = TextureCache::LoadTexture("../data/model/tree01.dds");

	mPlane = ModelCache::LoadModel("../data/model/plane.obj");
	mPlane->mNodes[0].texture = TextureCache::LoadTexture("../data/texture/stone.dds");

	mTexture = TextureCache::LoadTexture("../data/texture/style.dds");

	mShader = ShaderCache::CreateShader("ForwardVS", "ForwardPS");
	
	FrameBufferCreateInfo framebufferInfo{};
	framebufferInfo.Type = FRAMEBUFFER_MTR_DEPTHSTENCIL;
	framebufferInfo.BufferCount = 3;
	framebufferInfo.BufferFormat = Format::FORMAT_RGBA16_FLOAT;
	framebufferInfo.Width = v.w;
	framebufferInfo.Height = v.h;

	mFrameBuffer = BufferCache::CreateFrameBuffer(&framebufferInfo);

	mGBufferShader = ShaderCache::CreateShader("GBufferVS", "GBufferPS");
	mDeferredShader = ShaderCache::CreateShader("DeferredVS", "DeferredPS");

	//match gbuffer
	float size = 1.0f;

	//depend on HLSL side
	float quad[] = {
		-size,size,0.0f, 0.0f, 0.0f,
		size,size,0.0f, 1.0f, 0.0f,
		-size,-size,0.0f, 0.0f, 1.0f,
		size,-size,0.0f, 1.0f, 1.0f
	};

	std::vector<VertexAttrib> attbs = {
		{ 0, VertexFormat::FLOAT, 3, sizeof(float) * 5, 0 },
		{ 0, VertexFormat::FLOAT, 2, sizeof(float) * 5, 12 },
	};

	VertexBufferCreateInfo vertexInfo{};
	vertexInfo.AttribSize = attbs.size();
	vertexInfo.pAttrib = attbs.data();
	vertexInfo.pVertices = quad;
	vertexInfo.VerticesSize = sizeof(quad);
	vertexInfo.Type = BufferType::BUFFER_STATIC;

	mQuadBuffer = BufferCache::CreateVertexBuffer(&vertexInfo);

	mSky = new SkyCube;
}

void DeferredScene::UpdateScene(float delta)
{
	mCamera->ProcessFirstPerson(GInput, delta);

	pv[0] = mCamera->GetProjection();
	pv[1] = mCamera->GetView();
	mUbo0->Update(pv);

	mUbo1->Update(model.data());

	auto pos = mCamera->GetPostion();
	mUbo2->Update(&pos);
}

void DeferredScene::RenderScene()
{
	mUbo0->BindVS(0);
	mUbo1->BindVS(1);
	mUbo2->BindPS(2);

	//Record Start (GBuffer)
	mFrameBuffer->Bind();
	mFrameBuffer->Clear(0.0f, 0.0f, 0.0f, 1.0f);

	//Bind GBuffer Shader
	Matrix4x4 t;
	t.Scale(1);
	mUbo1->Update(t.data());
	mGBufferShader->Bind();

	mPlane->Render();
	int size = 1;
	for (int z = -size; z < size; ++z) {
		for (int x = -size; x < size; ++x) {
			Matrix4x4 m;
			m.Scale(3);
			m.Translate(x * 5, 0, z * 5);
			mUbo1->Update(m.data());
			mModel->Render();

		}
	}
	
	//Finish Record
	mFrameBuffer->UnBind();

	//Clear Main Back Buffer Depth and Color
	GContext->Clear(0.2, 0.2, 0.2, 1);

	for (uint i = 0; i < mFrameBuffer->GetTextureCount(); ++i) {
		Texture* gbuffer = mFrameBuffer->GetTexture(i);
		gbuffer->Bind(i);
	}

	//Draw Screen
	mDeferredShader->Bind();
	mQuadBuffer->DrawArray(Topology::TRIANGLE_STRIP, 0, 4);	

	PROFILE_START("blit");
	Viewport v;
	GContext->GetViewport(v);
	//TODO : Bit Operator Color or Depth Stencil
	mFrameBuffer->BlitDepthStencilBuffer(0, 0, v.w, v.h);
	PROFILE_END("blit");

	//then we can z fighting
	//sky z is 1.0 infinite far
	mSky->Render();

}

void DeferredScene::RenderSprite()
{
	//test 
	for (uint i = 0; i < mFrameBuffer->GetTextureCount(); ++i) {

		Texture* texture = mFrameBuffer->GetTexture(i);
		GSpriteBatch->Render(10, i * 230 + 20, 250, 220, texture);
	}

}

void DeferredScene::RenderText()
{

	char buffer[64];

	vec3f pos = mCamera->GetPostion();
	sprintf(buffer, "pos : %.3f %.3f %.3f", pos.x, pos.y, pos.z);
	G_FontBatch->Render(350, 30, buffer, vec4f(1));

	sprintf(buffer, "blit : %9.7lf", PROFILE_MS("blit"));
	G_FontBatch->Render(350, 60, buffer, vec4f(1));

}
