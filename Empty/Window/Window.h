#pragma once


#include <core/type.h>
#include <assert.h>
#include <string>
#include <window/painter.h>

#include <Windows.h>
//https://msdn.microsoft.com/en-us/library/windows/desktop/ms644931(v=vs.85).aspx
#define WM_USER_ENTEREVENT			WM_USER + 1

class Window
{
public:
	Window(Window* parent = NULL);
	Window(uint x, uint y, uint w, uint h, Window* parent = NULL);
	virtual~Window(){}


	uint x;
	uint y;
	uint width;
	uint height;
	HWND mHandle;
	Window* mParent;
	std::string mName = "none";

	//built in
	RECT	GetRect();

protected:
	virtual void Create();
	virtual void PreRegisterClass(WNDCLASS& wc);		//changeable by derived
	virtual void PreCreateStruct(CREATESTRUCT& cs);
	virtual LRESULT LocalWndProc(UINT msg, WPARAM wp, LPARAM lp);


	static LRESULT WINAPI GlobalWndProc(HWND handle, UINT msg, WPARAM wp, LPARAM lp);
};

