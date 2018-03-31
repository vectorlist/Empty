#include <Core/PCH.h>
#include <Renderer/Renderer.h>
#include <Window/Applicationinfo.h>
#include <Cache/SceneCache.h>
#include <Core/Input.h>

#include <Graphics/GL4/GLContext.h>
#include <Graphics/DX11/DXcontext.h>

#include <Renderer/Batch//GL4/GLDebugBatch.h>
#include <Renderer/Batch/DX11/DXDebugBatch.h>

#include <Renderer/Batch/GL4/GLFontBatch.h>
#include <Renderer/Batch/DX11/DXFontBatch.h>

#include <Renderer/Batch/SpriteBatch.h>

Renderer::Renderer()
	: mContext(nullptr), mSceneCache(nullptr), mInput(nullptr), mDebugBatch(nullptr), mFontBatch(nullptr)
{
}

Renderer::~Renderer()
{
	SAFE_DELETE(mSpriteBatch);
	SAFE_DELETE(mInput);
	SAFE_DELETE(mDebugBatch);
	SAFE_DELETE(mFontBatch);
	SAFE_DELETE(mContext);		//delete context  at last
}

void Renderer::Init(uint width, uint height, void* hwnd, bool vsync)
{
	auto info = GetSubSystem<ApplicationInfo>();
	info->hwnd = hwnd;

	//GRAPHICS APIT CONTEXT
	switch (info->apiType)
	{
	case GraphicAPI::OPENGL45:
		mContext = new GLContext(width, height, hwnd, vsync);
		break;
	case GraphicAPI::DIRECTX11:
		mContext = new DXContext(width, height, hwnd, vsync);
		break;
	}

	//DEBUG BATCH
	switch (info->apiType)
	{
	case GraphicAPI::OPENGL45:
		mDebugBatch = new GLDebugBatch;
		break;
	case GraphicAPI::DIRECTX11:
		mDebugBatch = new DXDebugBatch;
		break;
	default:
		ASSERT_MSG(0, "failed to create debug batch");
		break;
	}

	//FONT BATCH (TODO : Replace Internal Batches)
	switch (info->apiType)
	{
	case GraphicAPI::OPENGL45:
		mFontBatch = new GLFontBatch;
		mFontBatch->Init("../data/font/arial.ttf", 22);
		break;
	case GraphicAPI::DIRECTX11:
		mFontBatch = new DXFontBatch;
		mFontBatch->Init("../data/font/arial.ttf", 22);
		break;
	default:
		ASSERT_MSG(0, "failed to create font batch");
		break;
	}

	//Test Without Chooseing API
	mSpriteBatch = new SpriteBatch;

	mSceneCache = G_SceneCache;
	mInput = new Input(hwnd);
	
}

void Renderer::Update(float delta)
{
	if (mContext){
		mInput->ProcessInputs();
		mSceneCache->Update(delta);
	}
}

void Renderer::Render()
{
	if (mContext) {
		mContext->Clear(0.0f, 0.0f, 0.0f, 1.0f);

		mSceneCache->Render();

		mContext->SwapBuffer();
	}
}

void Renderer::Resize(int width, int height)
{
}
