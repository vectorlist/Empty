#include <PCH.h>
#include "FontBatchScene.h"

#include <renderer/batch/FontBatch.h>

#include <cache/buffercache.h>
#include <cache/shadercache.h>
#include <cache/texturecache.h>
#include <core/camera.h>

#include <graphics/model.h>
#include <core/input.h>

FontBatchScene::FontBatchScene()
{
}

void FontBatchScene::InitScene()
{
	Viewport v;
	G_Context->GetViewport(v);

	mCamera = new Camera(45.f, (float)v.w / (float)v.h);

	mModel = Model::CreateModelFromFile("../data/model/knot.obj");

	BufferCreateInfo info{};
	info.pData = nullptr;
	info.size = sizeof(Matrix4x4) * 3;
	info.type = BufferType::BUFFER_DYNAMIC;
	info.binding = BufferBinding::BIND_UNIFORM;

	mUbo = BufferCache::CreateUniformBuffer(info);

	mShader = ShaderCache::CreateSceneShader();
	mTexture = TextureCache::LoadTexture("../data/texture/style.dds");
}

void FontBatchScene::UpdateScene(float delta)
{
	mCamera->ProcessFirstPerson(GInput, delta);

	matrice.proj = mCamera->GetProjection();
	matrice.view = mCamera->GetView();
	matrice.model = Matrix4x4();
	mUbo->Update(&matrice);
}

void FontBatchScene::RenderScene()
{
	mShader->Bind();
	mUbo->BindVS(0);
	//TODO Texture seperate vs, ps
	mTexture->Bind(0);

	mModel->Render();

}

void FontBatchScene::RenderText()
{
	//test with profiler
	for(int i =0; i < 50; ++i)
		G_FontBatch->Render(10, i * 24, "ABDCEFGHIJKLMNOPQRSTUVWXYZ0123456789abcbefghijl", vec4f(1));
}
