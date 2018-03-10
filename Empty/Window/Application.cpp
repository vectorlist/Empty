#include <PCH.h>
#include <window/application.h>
#include <ECS/SubSystem.h>
#include <window/applicationinfo.h>

#include <window/renderwindow.h>
#include <window/titlebar.h>

#include <renderer/renderer.h>
//#include <graphics/scene.h>

Application::Application()
	: mIsRunning(true), mRenderWindow(NULL), mTitleBar(NULL)
{
	auto info = GetSubSystem<ApplicationInfo>();

	width = info->width;
	height = info->height;
	Create();
}

Application::~Application()
{
	SAFE_DELETE(mRenderWindow);
	SAFE_DELETE(mTitleBar);
}

void Application::PreRegisterClass(WNDCLASS& wc)
{
	wc.lpszClassName = TEXT("application");
}

void Application::PreCreateStruct(CREATESTRUCT& cs)
{
	auto info = GetSubSystem<ApplicationInfo>();

	cs.lpszName = info->appName.c_str();
	if ((info->flags & APP_CENTERPOS) != 0)
	{
		auto x = (GetSystemMetrics(SM_CXSCREEN) - cs.cx) / 2;
		auto y = (GetSystemMetrics(SM_CYSCREEN) - cs.cy) / 2;
		cs.x = x;
		cs.y = y;
	}
	if ((info->flags & APP_TITLEBAR) != 0)
	{
		//we have to WS_VISIBLE here
		//u need ::ShowWindow(this handle) outside
		cs.style = WS_POPUP;

		//increase this application window size (height only)
		height = height + TITLEBAR_FIXEDSIZE;
		cs.cy = height;
	}
}

LRESULT Application::LocalWndProc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		mTitleBar = new TitleBar(0, 0, width, TITLEBAR_FIXEDSIZE, this);

		mRenderWindow = new RenderWindow(0, TITLEBAR_FIXEDSIZE, width,
			height - TITLEBAR_FIXEDSIZE, this);
	}
	case WM_SIZE:
	{
		break;
	}
	case WM_NCHITTEST:
	{
		return HTCAPTION;
	}
	}
	return DefWindowProc(mHandle, msg, wp, lp);
}

int Application::Run()
{
	if (!mRenderWindow) {
		LOG_ERROR("failed to find render window");
	}
	//matching renderer and scene apit type
	Renderer* renderer = mRenderWindow->GetRenderer();

	ShowWindow(mHandle, TRUE);
	SetFocus(mRenderWindow->mHandle);
	SetForegroundWindow(mRenderWindow->mHandle);

	//::ShowCursor(FALSE);
	
	mTimer.Reset();

	MSG msg{};
	while (mIsRunning)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			if (msg.message == WM_QUIT) {
				mIsRunning = false;
			}
			mTimer.Begin();

			float delta = mTimer.GetDeltaTime();
			float fps = mTimer.fps;
		
			renderer->Update(delta);
			renderer->Render();
		}
	}

	return static_cast<int>(msg.lParam);
}

