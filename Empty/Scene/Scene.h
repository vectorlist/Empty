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
private:
	static std::map<uint, std::shared_ptr<Object>> 	mInternalObjects;
};

