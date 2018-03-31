#include <Core/PCH.h>
#include <core/input.h>
#include <Windows.h>

Input* Input::globalInput = nullptr;

Input::Input(void* hwnd)
	: hwnd((HWND)hwnd), firtstPress(false)
{
	globalInput = this;
}

Input::~Input()
{
}

vec2i Input::GetMousePos() const
{
	POINT p;
	::GetCursorPos(&p);
	ScreenToClient((HWND)hwnd, &p);
	return vec2i(p.x, p.y);
}

bool Input::IsKeyPressed(char c)
{
	if ((GetAsyncKeyState(c) & 0x8000) != 0)
		return true;
	return false;
}

void Input::ProcessInputs()
{
	mouse.pos = GetMousePos();
	if (IsKeyPressed(VK_LBUTTON))
	{
		mMouseDelta = mouse.pos - mLastPos;
	}
	mLastPos = mouse.pos;
}

vec2i& Input::GetMouseDelta()
{
	return mMouseDelta;
}

