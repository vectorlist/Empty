#pragma once

class Context
{
public:
	Context(uint width, uint height, void* hwnd, bool vsync);
	virtual~Context() {}
	
	//================== Graphics =======================
	virtual void Clear(float r, float g, float b, float a) = 0;
	virtual void SwapBuffer() = 0;

	virtual void SetViewport(const Viewport& vp) = 0;
	virtual void GetViewport(Viewport& vp) = 0;

	virtual void SetBlendState(bool enable) = 0;
	virtual void SetDepthStencil(bool enable) = 0;

	virtual void SetTexture(uint slot, Texture* texture) {}; // must be derived
	virtual void DrawArrays(VertexBuffer* buffer, uint count) {};

	//============== Expand Graphics Func ===============

	virtual void SetDepthStencilEx(DepthStencilState* stencilState) {};
	virtual void SetCullMode(CullMode mode) {};

	//============== Frame Buffer =======================
	

	//================== Internal =======================
	void*			GetHwnd() const { return mHwnd; }
	bool			GetVsync() const { return mVsync; }
	Context*		GetInstance() { return globalContext; }
	GraphicAPI		GetApiType() const { return mApiType; }
	uint			GetWidth() const { return mWidth; }
	uint			GetHeight() const { return mHeight; }
	virtual void*	GetDepthStencilView() { return nullptr; }
	virtual void*	GetDepthStencilTexture() { return nullptr; }
	virtual void*	GetRenderTargetView() { return nullptr; }

	static Context*				globalContext;		
	static ID3D11Device*		globalDXDevice;
	static ID3D11DeviceContext* globalDXDeviceContext;
	static HDC					globalGLDevice;
	static HGLRC				globalGLContext;
protected:

	void*	mHwnd;
	bool	mVsync;
	uint	mWidth;
	uint	mHeight;
	GraphicAPI mApiType;
};

#define GContext	Context::globalContext

#define G_DXDevice	Context::globalDXDevice
#define G_DXContext  Context::globalDXDeviceContext

#define G_GLDevice	Context::globalGLDevice 
#define G_GLContext	Context::globalGLContext
