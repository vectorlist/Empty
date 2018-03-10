#include <PCH.h>
#include <Cache/SceneCache.h>
#include <Scene/Scene.h>

Scene* SceneCache::currentScene = nullptr;
Scene* SceneCache::queuedScene = nullptr;

SceneCache::~SceneCache()
{
}

void SceneCache::SetQueueScene(const std::string& name)
{
	auto found = mScenes.find(name);
	if (found == mScenes.end()) {
		
		LOG_FATAL("failed to find scene");
	}

	queuedScene = found->second.get();
	queuedScene->Init();

	currentScene = queuedScene;

}

void SceneCache::Update(float delta)
{
	if (currentScene) currentScene->Update(delta);
}

void SceneCache::Render()
{
	if (currentScene) currentScene->Render();
}

void SceneCache::ReleaseAll()
{
}
