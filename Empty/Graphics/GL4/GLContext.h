#pragma once

#include <graphics/context.h>

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

	//================== EXPAND API ================
	void SetDepthStencilEx(DepthStencilState* state);
	void SetCullMode(CullMode mode) override;

	//=============== FRAME BUFFER =================


	//============================================

	HDC		mDc;			//handle of opengl device context
	HGLRC	mRc;			//handle of Graphic redering context
	
	std::vector<int>	contextAttribs;
	std::vector<int>	pixelFormatAttribs;
	std::vector<float>	pixelFormatFloatAttribs;
	std::string mCoreInfo;
private:
	//================ internal ====================
	void Init();
	bool InitExtension();
};

