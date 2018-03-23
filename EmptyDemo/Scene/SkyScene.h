#pragma once

#include <Scene/scene.h>

class SkyScene : public Scene
{
public:
	SkyScene(){}
	virtual ~SkyScene();

	virtual void InitScene();
	virtual void UpdateScene(float delta);
	virtual void RenderScene();

	virtual void RenderText();

	Camera* mCamera;
	Matrices matrices;
	UniformBuffer* mUbo;		//Main UBO

	//Test
	Environment* mEnviron;

	Model* mModel;
	Shader* mShader;
	Texture* mTexture;
	Terrain* mTerrain;
};