#include <Core/PCH.h>
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

//TODO : More Info EX : EQUAL LESS OR MORE
void GLContext::SetDepthStencil(bool enable)
{
	//TODO : Replace TO Depth Func
	if (enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void GLContext::DrawArrays(VertexBuffer* buffer, uint count)
{
	//TODO : Raster Topology State
	//glDrawArrays(,)
}

void GLContext::SetDepthStencilEx(DepthStencilState* state)
{
	switch (state->Enabled)
	{
	case true:
		glEnable(GL_DEPTH_TEST);
		break;
	case false:
		glDisable(GL_DEPTH_TEST);
		break;
	default:
		break;
	}
	switch (state->Mask)
	{
	case DepthMask::NONE:
		glDepthMask(false);
		break;
	case DepthMask::ALL:
		glDepthMask(true); 
		break;
	default:
		break;
	}
	switch (state->Func)
	{
	case DepthFunc::LESS:
		glDepthFunc(GL_LESS);
		break;
	case DepthFunc::LESS_EQUAL:
		glDepthFunc(GL_LEQUAL);
		break;
	default:
		break;
	}

}

void GLContext::SetCullMode(CullMode mode)
{
	switch (mode)
	{
	case CullMode::FRONT:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		break;
	case CullMode::BACK:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		break;
	case CullMode::FRONT_AND_BACK:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
	case CullMode::NONE:
		glDisable(GL_CULL_FACE);
		break;
	}
}


void GLContext::Init()
{
	if (!InitExtension()) return;

	mDc = GetDC((HWND)mHwnd);

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
			WGL_CONTEXT_RELEASE_BEHAVIOR_ARB, WGL_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB,
			0
		};
		
		int pixelFormat;
		int numFormats;	
		if (!wglChoosePixelFormatARB(mDc, pixelFormatAttribs.data(),
			pixelFormatFloatAttribs.data(), 1, &pixelFormat, (uint*)&numFormats))
		{
			LOG_FATAL("failed to find pixel format from attribs");
		}

		PIXELFORMATDESCRIPTOR pfd{};		//get more infomation of pixel format
		DescribePixelFormat(mDc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

		if (!SetPixelFormat(mDc, pixelFormat, &pfd))
			LOG_ERROR("failed to set pixel format");

		mRc = wglCreateContextAttribsARB(mDc, 0, contextAttribs.data());

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

	if (GL_ARB_clip_control) {
		//https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_clip_control.txt
		glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
		glDepthRange(0.0f, 1.0f);
	}

	//------------------ gl state ---------------------------
	wglSwapIntervalEXT(mVsync ? 1 : 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);							//same clock wise to directx

	glViewport(0, 0, (int)mWidth, (int)mHeight);
	glClearColor(0, 0, 0, 1);
}


bool GLContext::InitExtension()
{
	HINSTANCE inst = GetModuleHandle(NULL);										
	HWND tempWindow = CreateWindowEx(NULL,
		"STATIC", "temp", WS_POPUP, 0, 0,
		0, 0, NULL, NULL, inst, NULL);

	mDc = GetDC(tempWindow);

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

	int pixelFormat = ::ChoosePixelFormat(mDc, &pfd);

	if (pixelFormat <= 0) return false;

	if (!SetPixelFormat(mDc, pixelFormat, &pfd)) return false;

	HGLRC tempRc = wglCreateContext(mDc);
	wglMakeCurrent(mDc, tempRc);

	if (!WGL::InitWGL()) {
		return false;
	}
	int extNum;
	glGetIntegerv(GL_MAX_EXT, &extNum);

	//destroy temp window
	wglMakeCurrent(NULL, NULL);				//disconnect
	wglDeleteContext(tempRc);				//delete temp render context
	DestroyWindow(tempWindow);				//delete window
	mDc = NULL;								//device context to null

	return true;
}
