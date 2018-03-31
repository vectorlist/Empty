#pragma once

#include <Scene/scene.h>

class DeferredScene : public Scene
{
public:
	DeferredScene();
	virtual ~DeferredScene();

	virtual void InitScene();
	virtual void UpdateScene(float delta);
	virtual void RenderScene();
	virtual void RenderSprite() override;
	virtual void RenderText();

	Camera* mCamera;

	Matrix4x4 pv[2];
	Matrix4x4 model;

	UniformBuffer* mUbo0;		//Main UBO
	UniformBuffer* mUbo1;

	//additional ubo
	UniformBuffer* mUbo2;

	//light
	UniformBuffer* mUbo3;

	//Test
	Environment* mEnviron;

	Model* mModel;
	Model* mPlane;
	Shader* mShader;
	Shader* mInstanceShader;
	Texture* mTexture;
	Texture* mTexture0;

	std::vector<Matrix4x4> instanceData;

	FrameBuffer*	mFrameBuffer;
	Shader*			mGBufferShader;
	Shader*			mDeferredShader;

	VertexBuffer*   mQuadBuffer;

	//Test
	Environment*	mSky;
};