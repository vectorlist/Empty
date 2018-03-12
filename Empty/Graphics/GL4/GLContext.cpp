#include <PCH.h>
#include <Graphics/GL4/GLContext.h>
#include <Graphics/GL4/GLConfig.h>

#define WIN32_LEAN_AND_MEAN			
#include <Windows.h>


GLContext::GLContext(uint width, uint height, void* hwnd, bool vsync)
	: Context(width, height, hwnd, vsync)
{
	mApiType = GraphicAPI::OPENGL45;
	Init();
	globalGLDevice = mDc;
	globalGLContext = mRc;
	//mSwapChain = new SwapChain;
}

GLContext::~GLContext()
{
	if (!mHwnd) return;
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(mRc);
	ReleaseDC((HWND)mHwnd, mDc);
}

void GLContext::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
}

void GLContext::SwapBuffer()
{
	SwapBuffers(mDc);
	//mSwapChain->Present();
}

void GLContext::SetViewport(const Viewport& vp)
{
	glViewport(vp.x, vp.y, vp.w, vp.h);
}

void GLContext::GetViewport(Viewport& vp)
{
	glGetIntegerv(GL_VIEWPORT, (GLint*)&vp);
}

void GLContext::SetBlendState(bool enable)
{
	if (enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}

void GLContext::SetDepthStencil(bool enable)
{
	if (enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void GLContext::Init()
{
	if (!InitExtension()) return;

	mDc = GetDC((HWND)mHwnd);

	//no glew
	if (WGL_ARB_create_context && WGL_ARB_pixel_format)
	{
		LOG << "support wglew arb extension" << ENDN;
		pixelFormatAttribs =
		{
			WGL_SUPPORT_OPENGL_ARB,		GL_TRUE,
			WGL_COLOR_BITS_ARB,			32,
			WGL_DEPTH_BITS_ARB,			24,
			WGL_STENCIL_BITS_ARB,		8,
			WGL_DOUBLE_BUFFER_ARB,		GL_TRUE,
			0
		};

		pixelFormatFloatAttribs = { 0.0f };

		contextAttribs =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 5,
			WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			//WGL_CONTEXT_RELEASE
			//TODO : newset opengl can avoid glflush with WGL_CONTEXT_RELEASE_BEHAVIOUR
			//disable glFlush
#ifndef _GLEW
			WGL_CONTEXT_RELEASE_BEHAVIOR_ARB, WGL_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB,
#endif
			//0x2098, 0x0000,
			0
		};
		
		//TODO : GL_KHR_debug

		int pixelFormat;
		int numFormats;	
		if (!wglChoosePixelFormatARB(mDc, pixelFormatAttribs.data(),
			pixelFormatFloatAttribs.data(), 1, &pixelFormat, (uint*)&numFormats))
		{
			LOG_FATAL("failed to find pixel format from attribs");
		}

		PIXELFORMATDESCRIPTOR pfd{};		//get more infomation of pixel format
		DescribePixelFormat(mDc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

		//set pixel to device context
		if (!SetPixelFormat(mDc, pixelFormat, &pfd))
			LOG_ERROR("failed to set pixel format");

		mRc = wglCreateContextAttribsARB(mDc, 0, contextAttribs.data());
		//WGLEW_ARB_context_flush_control
		//connect rc and dc
		wglMakeCurrent(mDc, mRc);
	}
	else {
		LOG_ERROR("GPU dosent support wglew arb extension");
	}

	const char* version = (char*)glGetString(GL_VERSION);
	const char* vendor = (char*)glGetString(GL_VENDOR);
	const char* glslVer = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

	char apiinfo[128];
	sprintf_s(apiinfo, "API : OpenGL %s GLSL : %s", version, glslVer);
	mCoreInfo = std::string(apiinfo);

	LOG << mCoreInfo << ENDN;

	//check extension GL_ARB_clip_control to control of clip space
	if (GL_ARB_clip_control) {
		//matching direct x clip space
		//https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_clip_control.txt
		glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	}

	//------------------ gl state ---------------------------
	wglSwapIntervalEXT(mVsync ? 1 : 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glDisable(GL_MULTISAMPLE);
	//glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);							//same clock wise to directx

	glViewport(0, 0, (int)mWidth, (int)mHeight);
	glClearColor(0, 0, 0, 1);
	//GL_ARB_texture_non_power_of_two
}


bool GLContext::InitExtension()
{
	//replace to pure extension
	//out if we glew init already
	HINSTANCE inst = GetModuleHandle(NULL);	// get current program address

											//"STATIC" keyword for lpClassName pre defined so u dont need register WNDCLASS
											//CreateWindowA is not unicode function using CreateWindow if u using unicode
											//and "Temp" or TEXT("temp") for unicode
	HWND tempWindow = CreateWindowEx(NULL,
		"STATIC", "temp", WS_POPUP, 0, 0,
		0, 0, NULL, NULL, inst, NULL);

	//get dc from temp
	mDc = GetDC(tempWindow);

	//choose pixel for surface
	PIXELFORMATDESCRIPTOR pfd{};
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cAlphaBits = 8;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	//system call pixel value (return 0 is unable to use)
	int pixelFormat = ::ChoosePixelFormat(mDc, &pfd);

	if (pixelFormat <= 0) return false;

	//set pixel format to temp device context
	if (!SetPixelFormat(mDc, pixelFormat, &pfd)) return false;

	//well create temp Render context
	HGLRC tempRc = wglCreateContext(mDc);

	//connect btw dc, temprc
	wglMakeCurrent(mDc, tempRc);

	//replace glew to wglext(for WGL_CONTEXT_RELEASE_BEHAVIOUR)
#ifdef _GLEW
	if (glewInit() != GLEW_OK) {
		return false;
	};
#else
	if (!WGL::InitWGL()) {
		return false;
}
#endif // _GLEW

	
	int extNum;
	glGetIntegerv(GL_MAX_EXT, &extNum);

	//destroy temp window
	wglMakeCurrent(NULL, NULL);				//disconnect
	wglDeleteContext(tempRc);				//delete temp render context
	DestroyWindow(tempWindow);				//delete window
	mDc = NULL;								//device context to null

	return true;
}
