#include <PCH.h>
#include "FontBatchScene.h"

#include <renderer/batch/FontBatch.h>

#include <cache/buffercache.h>
#include <cache/shadercache.h>
#include <cache/texturecache.h>
#include <core/camera.h>

#include <core/input.h>

#include <Image/Image.h>
#include <Cache/ModelCache.h>

#include <Graphics/Context.h>

FontBatchScene::FontBatchScene()
{
}

void FontBatchScene::InitScene()
{
	Viewport v;
	GContext->GetViewport(v);

	mCamera = new Camera(45.f, (float)v.w / (float)v.h);

	mModel = ModelCache::LoadModel("../data/model/knot.obj");

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
	for(int i =0; i < 1; ++i)
		G_FontBatch->Render(10, i * 24, "ABDCEFGHIJKLMNOPQRSTUVWXYZ0123456789abcbefghijl", vec4f(1));

	G_FontBatch->Render(10, 10, "hello", vec4f(1));
}
