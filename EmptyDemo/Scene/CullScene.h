#pragma once

#include <Scene/scene.h>
#include <Engine/CullManager.h>

class CullScene : public Scene
{
public:
	CullScene() {}
	virtual ~CullScene();

	virtual void InitScene();
	virtual void UpdateScene(float delta);
	virtual void RenderScene();
	
	virtual void RenderText();

	Camera* mCamera;

	Matrix4x4 pv[2];
	Matrix4x4 model;

	UniformBuffer* mUbo0;		//Main UBO
	UniformBuffer* mUbo1;


	Model* mModel;
	Shader* mShader;
	Shader* mInstanceShader;
	

	std::vector<Object*> mObject;
	CullManager mCull;

	//Culled objects instance data
	std::vector<Matrix4x4> instanceData;
	static uint culledCount;

	//reserve as total object count
	Matrix4x4 queryData[10000];

	Environment* mSky;
	
};