#include <PCH.h>
#include <window/renderwindow.h>
#include <System/SubSystem.h>
#include <window/applicationinfo.h>


RenderWindow::RenderWindow(uint x, uint y, uint width, uint height, Window* parent)
	: Window(x,y, width, height, parent)
{
	this->Create();
	this->CreateRenderer();
}

void RenderWindow::CreateRenderer()
{
	auto info = GetSubSystem<ApplicationInfo>();
	info->hwnd = this->mHandle;         //important to make renderer
	mRenderer.Init(width, height, mHandle, info->vsync);
}

void RenderWindow::PreRegisterClass(WNDCLASS &wc)
{
	wc.lpszClassName = TEXT("render window");
	//wc.style |= CS_OWNDC;
	//wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wc.style = CS_OWNDC;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
}

void RenderWindow::PreCreateStruct(CREATESTRUCT &cs)
{
	if (mParent) {
		cs.style |= WS_VISIBLE;
	}
	else {
		cs.style = WS_POPUP;
	}

}

LRESULT RenderWindow::LocalWndProc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		int w = LOWORD(lp);
		int h = HIWORD(lp);
		break;
	} 
	case WM_KEYDOWN:
	{
		if (wp == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		break;
	}
	case WM_MOUSEACTIVATE:
	{
		SetForegroundWindow(mHandle);
		SetFocus(mHandle);
		break;
	}
	}
	return ::DefWindowProc(mHandle, msg, wp, lp);
}

Renderer* RenderWindow::GetRenderer()
{
	return &mRenderer;
}
