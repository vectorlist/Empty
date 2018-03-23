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

#include <Graphics/Context.h>

ImageScene::ImageScene()
{
}

void ImageScene::InitScene()
{
	Viewport v;
	GContext->GetViewport(v);

	mCamera = CreateSceneCamera("Main Camera", 45.f, (float)v.w / (float)v.h,0.1f, 1000.f);
	mCamera2 = CreateSceneCamera("Sub Camera",60.f, (float)v.w / (float)v.h);
	currentCamera = mCamera;

	mModel = ModelCache::LoadModel("../data/model/knot.obj");

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
	terrainInfo.Filename = "../data/texture/256.bmp";
	terrainInfo.HeightScale = 100;
	terrainInfo.CellScale = 1;
	terrainInfo.nTiles = 1;
	terrainInfo.nSmooth = 2;

	mTerrain = TerrainCache::CreateTerrain("main terrain", &terrainInfo);
	//mTerrain->mCamera = mCamera;
}

void ImageScene::UpdateScene(float delta)
{
	if(GInput->IsKeyPressed('1')) {
		currentCamera = mCamera;
	}
	if (GInput->IsKeyPressed('2')) {
		currentCamera = mCamera2;
	}
	currentCamera->ProcessFirstPerson(GInput, delta);

	matrice.proj = currentCamera->GetProjection();
	matrice.view = currentCamera->GetView();
	matrice.model = Matrix4x4();
	mUbo->Update(&matrice);
}

#include <Physics/Tree/QuadTree.h>

void ImageScene::RenderScene()
{
	mUbo->BindVS(0);


	//in bound
	Frustum frustum;
	frustum.Extract(mCamera->GetProjection(), mCamera->GetView());

	mShader->Bind();
	mTexture->Bind(0);

	mTerrain->Render(&frustum);
	//mTerrain->Render(nullptr);

	//for (int i = 0; i < mTerrain->mTree.mVisibleQueryNodesCount; ++i)
	//{
	//	auto aabb = &mTerrain->mTree.mQN[i]->mAABB;
	//	G_DebugBatch->RenderAABB(aabb, Matrix4x4(), vec3f(1, 0, 0));
	//}

	//auto qt = mTerrain->mQuadTree.mChild[0];
	//auto* qt = &mTerrain->mQuadTree;

	//auto* aabb = &qt->mAABB;

	//G_DebugBatch->RenderAABB(aabb, Matrix4x4(), vec3f(1));
}

void ImageScene::RenderText()
{
	char buffer[64];
	vec3f pos = mCamera->GetPostion();
	sprintf(buffer, "pos : %.3f %.3f %.3f", pos.x, pos.y, pos.z);
	G_FontBatch->Render(10, 10, buffer, vec4f(1));

	uint nodes = mTerrain->mTree.mVisibleQueryNodesCount;
	sprintf(buffer, "Query Visvle Vertices : %d Query Nodes : %d", mTerrain->result.renderedVertices, nodes);
	G_FontBatch->Render(10, 70, buffer, vec4f(1));
}
