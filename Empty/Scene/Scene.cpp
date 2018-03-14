#include <PCH.h>
#include <scene/scene.h>
#include <core/object.h>

#include <renderer/batch/debugbatch.h>
#include <renderer/batch/FontBatch.h>

std::map<uint, std::shared_ptr<Object>> Scene::mInternalObjects;

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	InitScene();
}

void Scene::Update(float delta)
{
	UpdateScene(delta);
}

void Scene::Render()
{
	RenderScene();

	//priority DEBUG > SPRITE > FONT
	//Gather Debug Batch
	//G_DebugBatch->RenderBatch();

	//TODO : sprite batch
	RenderSprite();

	//Font Batch
	if (G_FontBatch) {
		G_FontBatch->BeginBatch();
		RenderText();
		G_FontBatch->EndBatch();
	}

	G_DebugBatch->RenderBatch();
}

Object* Scene::CreateSceneObject()
{
	std::shared_ptr<Object> obj = std::make_shared<Object>();
	uint id = obj->GetId();

	auto found = mInternalObjects.find(id);
	if (found != mInternalObjects.end()) {
		return static_cast<Object*>(found->second.get());
	}
	//not matched
	mInternalObjects.insert(std::make_pair(id, obj));
	return static_cast<Object*>(obj.get());
}