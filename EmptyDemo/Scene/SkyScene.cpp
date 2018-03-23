#include <PCH.h>
#include <Scene/SkyScene.h>

#include <Core/Camera.h>
#include <Core/Input.h>
#include <Graphics/Context.h>

#include <Cache/BufferCache.h>

#include <Renderer/Batch/FontBatch.h>
#include <Renderer/Environment.h>

#include <Cache/ModelCache.h>
#include <Cache/ShaderCache.h>
#include <Cache/TextureCache.h>
#include <Cache/TerrainCache.h>

SkyScene::~SkyScene()
{
}


void SkyScene::InitScene()
{
	Viewport v;
	GContext->GetViewport(v);

	mCamera = CreateSceneCamera("Main Camera", 45.f, (float)v.w / (float)v.h, 0.1f, 1000.f);

	BufferCreateInfo info{};
	info.pData = nullptr;
	info.size = sizeof(Matrix4x4) * 3;
	info.type = BufferType::BUFFER_DYNAMIC;
	info.binding = BufferBinding::BIND_UNIFORM;

	mUbo = BufferCache::CreateUniformBuffer(info);

	mEnviron = new SkyCube;

	mModel = ModelCache::LoadModel("../data/model/knot.obj");
	mShader = ShaderCache::CreateSceneShader();
	mTexture = TextureCache::LoadTexture("../data/texture/stone.dds");

	TerrainCreateInfo terrainInfo{};
	terrainInfo.Filename = "../data/texture/128.bmp";
	terrainInfo.HeightScale = 10;
	terrainInfo.nSmooth = 2;
	terrainInfo.CellScale = 2;
	terrainInfo.nTiles = 1;
	mTerrain = TerrainCache::CreateTerrain("main terrain",&terrainInfo);
}

void SkyScene::UpdateScene(float delta)
{

	mCamera->ProcessFirstPerson(GInput, delta);

	matrices.proj = mCamera->GetProjection();
	matrices.view = mCamera->GetView();
	matrices.model = Matrix4x4();
	mUbo->Update(&matrices);
}

void SkyScene::RenderScene()
{
	mUbo->BindVS(0);

	mEnviron->Render();
	//mEnviron->Render();

	mShader->Bind();
	mTexture->Bind(0);
	mModel->Render();

	Frustum frustum;

	frustum.Extract(matrices.proj, matrices.view);

	mTerrain->Render(&frustum);
}

void SkyScene::RenderText()
{
	char buffer[64];
	vec3f pos = mCamera->GetPostion();
	sprintf(buffer, "pos : %.3f %.3f %.3f", pos.x, pos.y, pos.z);
	G_FontBatch->Render(10, 10, buffer, vec4f(1));

	sprintf(buffer, "Sky Cube Render");
	G_FontBatch->Render(10, 70, buffer, vec4f(1));
}
