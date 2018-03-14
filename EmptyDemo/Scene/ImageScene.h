#pragma once

#include <scene/scene.h>

class ImageScene : public Scene
{
public:
	ImageScene();

	virtual void InitScene();
	virtual void UpdateScene(float delta);
	virtual void RenderScene();

	virtual void RenderText();

	Camera* mCamera;
	Model* mModel;

	Texture* mTexture;
	Shader* mShader;

	//ubo data
	Matrices matrice;
	UniformBuffer* mUbo;	//0 slot

	Texture* mImageTexture;

	Terrain* mTerrain;

};