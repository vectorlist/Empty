#pragma once

#include <graphics/context.h>

class SwapChain;
class GLContext : public Context
{
public:
	GLContext(uint width, uint height, void* hwnd, bool vsync);
	~GLContext();

	//================== API =======================
	void Clear(float r, float g, float b, float a) override;
	void SwapBuffer() override;

	void SetViewport(const Viewport& vp) override;
	void GetViewport(Viewport& vp) override;

	void SetBlendState(bool enable) override;
	void SetDepthStencil(bool enable) override;

	void DrawArrays(VertexBuffer* buffer, uint count);

	//============================================

	HDC		mDc;			//handle of opengl device context
	HGLRC	mRc;			//handle of Graphic redering context
	
	std::vector<int>	contextAttribs;
	std::vector<int>	pixelFormatAttribs;
	std::vector<float>	pixelFormatFloatAttribs;
	std::string mCoreInfo;
	SwapChain* mSwapChain;
private:
	//================ internal ====================
	void Init();
	bool InitExtension();
};


//Accepted as an attribute name in the <*attrib_list> argument to
//wglCreateContextAttribsARB :
//
//WGL_CONTEXT_RELEASE_BEHAVIOR_ARB                    0x2097
//
//Accepted as an attribute value for WGL_CONTEXT_RELEASE_BEHAVIOR_ARB in
//the <*attrib_list> argument to wglCreateContextAttribsARB :
//
//WGL_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB               0x0000
//WGL_CONTEXT_RELEASE_BEHAVIOR_FLUSH_ARB              0x2098
