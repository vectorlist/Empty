#include <Core/PCH.h>
#include "window.h"

Window::Window(Window* parent) 
	: x(0), y(0),width(0), height(0), mHandle(NULL), mParent(parent)
{

}

Window::Window(uint x, uint y, uint w, uint h, Window* parent)
	: x(x), y(y), width(w), height(h), mHandle(NULL), mParent(parent)
{
}

RECT Window::GetRect()
{
	RECT rect;
	::GetClientRect(mHandle, &rect);
	return rect;
}

void Window::Create()
{
	//create window by default
	HINSTANCE inst = ::GetModuleHandle(NULL);
	WNDCLASS wc{};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = inst;
	wc.lpszClassName = TEXT("win32++");
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpfnWndProc = Window::GlobalWndProc;
	wc.hbrBackground = CreateSolidBrush(RGB(80, 80, 80));
	wc.lpszMenuName = NULL;

	//u can change wc by derived
	this->PreRegisterClass(wc);

	//check overlap wc name
	if (!GetClassInfo(inst, wc.lpszClassName, &wc))
	{
		if (!RegisterClass(&wc))
			assert(0 && "invalid class name. exist already");
	}

	CREATESTRUCT cs{};
	cs.hInstance = wc.hInstance;
	cs.lpszClass = wc.lpszClassName;
	cs.x = x;
	cs.y = y;
	cs.cx = width;
	cs.cy = height;
	cs.lpCreateParams = this;			//for GetLongPtr
	cs.hMenu = NULL;
	cs.dwExStyle = NULL;
	cs.lpszName = mName.c_str();
	//set Parent
	cs.hwndParent = mParent ? mParent->mHandle : NULL;				

	DWORD singleStyle = WS_OVERLAPPED | WS_THICKFRAME;
	DWORD childStyle = WS_CHILD;
	cs.style = mParent ? childStyle : singleStyle;
	//u can change cs by derived
	PreCreateStruct(cs);

	mHandle = CreateWindowEx(
		cs.dwExStyle,
		cs.lpszClass,
		cs.lpszName,
		cs.style,
		cs.x, cs.y, cs.cx, cs.cy,
		cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams
	);
}

void Window::PreRegisterClass(WNDCLASS& wc){ /*by derived*/}

void Window::PreCreateStruct(CREATESTRUCT& cs) {/*by derived*/}

LRESULT Window::LocalWndProc(UINT msg, WPARAM wp, LPARAM lp)
{
	//defualt
	return DefWindowProc(mHandle, msg, wp, lp);
}

LRESULT Window::GlobalWndProc(HWND handle, UINT msg, WPARAM wp, LPARAM lp)
{
	//get window pointer from userdata
	Window* window = (Window*)GetWindowLongPtr(handle, GWLP_USERDATA);

	//when window created
	if (msg == WM_NCCREATE)
	{
		//set window from lp
		window = (Window*)((CREATESTRUCT*)lp)->lpCreateParams;
		//set handle to relative window
		window->mHandle = handle;
		//register to userdata 
		SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)window);
	}

	//ok if we have
	if (window)
		return window->LocalWndProc(msg, wp, lp);
	
	return ::DefWindowProc(handle, msg, wp, lp);
}