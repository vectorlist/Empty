#pragma once

#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#include <memory>

#define	TITLEBAR_COLOR			RGB(55,55,55)
#define	TITLEBAR_LCOLOR			RGB(100,100,100)
#define	BUTTON_COLOR			RGB(55,55,55)
#define BUTTON_LCOLOR			RGB(110,110,100)

#include <window/gdiobject.h>


class Painter
{
public:
	Painter(HWND handle);
	~Painter();

	void SetBrush(Brush& brush) { this->brush = brush; } //ptr dosent count
	void SetPen(Pen& pen);
	void SetFont(Font& font);

	void DrawLine(const Point& a, const Point& b);
	void DrawString(const std::string& code, RECT& rect, UINT flag);
	void DrawString(const std::string& code, RECT& rect, UINT flag, Color& color);

	//gdi
	HDC			dc;
	PAINTSTRUCT ps;
	Brush		brush;

	HWND handle;
};


#include <iostream>

inline std::ostream& operator<<(std::ostream& os, const RECT& rect)
{
	os << "Rect(" << rect.left << ", " << rect.top << ", " <<
		rect.right - rect.left << ", " << rect.bottom - rect.top << ")" << "\n";
	return os;
}

