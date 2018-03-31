#pragma once

#include <core/singleton.h>

class Scene;
class SceneCache : public Singleton<SceneCache>
{
public:
	SceneCache() {}
	~SceneCache();

	template <typename T>
	static Scene*	AddScene(const std::string& name);
	static void		SetQueueScene(const std::string& name);

	void Update(float delta);
	void Render();
	void ReleaseAll();

private:
	static std::map<std::string, std::shared_ptr<Scene>> mScenes;

	static Scene* currentScene;
	static Scene* queuedScene;
};

#define G_SceneCache SceneCache::GetInstance()

template<typename T>
inline Scene* SceneCache::AddScene(const std::string& name)
{
	static_assert(std::is_base_of<Scene, T>::value, "invliad scene class");
	auto found = mScenes.find(name);
	if (found == mScenes.end())
	{
		auto scene = std::make_shared<T>();
		mScenes.insert(std::make_pair(name, scene));
		return scene.get();
	}
	return found->second.get();
}
