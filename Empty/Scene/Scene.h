#pragma once

#include <core/type.h>

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init()					final;
	virtual void Update(float delta)	final;
	virtual void Render()				final;
	virtual void Release() {};

	//================ Derived ===================

	virtual void InitScene() {};
	virtual void UpdateScene(float delta) {};
	virtual void RenderScene() {};
	
	virtual void RenderSprite() {};		
	virtual void RenderText() {};
	//virtual void RenderDebug() {};
	
	static Object* CreateSceneObject();
	static Camera* CreateSceneCamera(const std::string& name, float fovy, float aspect, float znear = 0.1f, float zfar = 1000.f);
private:
	static std::map<uint, std::shared_ptr<Object>> 			mInternalObjects;
	static std::map<std::string, std::shared_ptr<Camera>>	mInternalCameras;
};

