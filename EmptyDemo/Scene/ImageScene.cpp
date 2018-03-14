#include <PCH.h>
#include <Scene/ImageScene.h>

#include <renderer/batch/FontBatch.h>
#include <renderer/batch/DebugBatch.h>

#include <cache/buffercache.h>
#include <cache/shadercache.h>
#include <cache/texturecache.h>
#include <core/camera.h>

#include <core/input.h>

#include <Image/Image.h>

#include <Cache/TerrainCache.h>
#include <Cache/ModelCache.h>


ImageScene::ImageScene()
{
}

void ImageScene::InitScene()
{
	Viewport v;
	G_Context->GetViewport(v);

	mCamera = new Camera(45.f, (float)v.w / (float)v.h);

	mModel = ModelCache::LoadModel("../data/model/plane.obj");

	BufferCreateInfo info{};
	info.pData = nullptr;
	info.size = sizeof(Matrix4x4) * 3;
	info.type = BufferType::BUFFER_DYNAMIC;
	info.binding = BufferBinding::BIND_UNIFORM;

	mUbo = BufferCache::CreateUniformBuffer(info);

	mShader = ShaderCache::CreateSceneShader();
	mTexture = TextureCache::LoadTexture("../data/texture/style.dds");

	mImageTexture = TextureCache::LoadTextureImage("../data/texture/ground.jpg");

	TerrainCreateInfo terrainInfo{};
	terrainInfo.fileName = "../data/texture/128.bmp";
	terrainInfo.heightScale = 10;
	terrainInfo.cellScale = 1;
	terrainInfo.nTiles = 1;
	terrainInfo.nSmooth = 2;

	mTerrain = TerrainCache::CreateTerrain("main terrain", &terrainInfo);



}

void ImageScene::UpdateScene(float delta)
{
	mCamera->ProcessFirstPerson(GInput, delta);

	matrice.proj = mCamera->GetProjection();
	matrice.view = mCamera->GetView();
	matrice.model = Matrix4x4();
	mUbo->Update(&matrice);
}

void ImageScene::RenderScene()
{
	//mShader->Bind();
	mUbo->BindVS(0);
	//TODO Texture seperate vs, ps
	//mTexture->Bind(0);
	//mImageTexture->Bind(0);

	//mModel->Render();


	mTerrain->TestRender();

	AABB aabb(mTerrain->mMin, mTerrain->mMax);
	G_DebugBatch->RenderAABB(&aabb, Matrix4x4(), vec3f(1));
}

void ImageScene::RenderText()
{
	char buffer[64];
	vec3f pos = mCamera->GetPostion();
	sprintf(buffer, "pos : %.3f %.3f %.3f", pos.x, pos.y, pos.z);
	G_FontBatch->Render(10, 10, buffer, vec4f(1));
}
