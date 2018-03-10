#pragma once

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init(uint width, uint height, void* hwnd, bool vsync);
	void Update(float delta);
	void Render();
	void Resize(int width, int height);

	
	Context* mContext;
	SceneCache* mSceneCache;
	Input* mInput;

	uint mWidth;
	uint mHeight;

	//TODO 
	//BVH Object

	//Visible Culling

	//TODO : Batch
	//static Debug Renderer
	//static Sprite Batch
	//static Text Batch
	DebugBatch*		mDebugBatch;
	FontBatch*		mFontBatch;
};
