#include <PCH.h>
#include "graphics/context.h"

Context*				Context::globalContext = nullptr;
ID3D11Device*			Context::globalDXDevice = nullptr;
ID3D11DeviceContext*	Context::globalDXDeviceContext = nullptr;
HDC						Context::globalGLDevice = nullptr;
HGLRC					Context::globalGLContext = nullptr;

Context::Context(uint width, uint height, void* hwnd, bool vsync)
	: mWidth(width), mHeight(height), mHwnd(hwnd), mVsync(vsync)
{
	if (globalContext) {
		ASSERT_MSG(0, "invalid multiple context");
	}
	globalContext = this;
}
