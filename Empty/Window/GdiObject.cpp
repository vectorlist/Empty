#include <PCH.h>
#include <window/painter.h>

Painter::Painter(HWND handle) : handle(handle)
{
	dc = BeginPaint(handle, &ps);	
}

Painter::~Painter()
{
	EndPaint(handle, &ps);
}

void Painter::SetPen(Pen& pen)
{
	::SelectObject(dc, pen);
}

void Painter::SetFont(HFont& font)
{
	::SelectObject(dc, font);
}

void Painter::DrawLine(const Point& a, const Point& b)
{
	MoveToEx(dc, a.x, a.y, NULL);
	LineTo(dc, b.x, b.y);
}

void Painter::DrawString(const std::string& code, RECT& rect, UINT flag)
{
	DrawTextA(dc, code.c_str(), -1, &rect, flag);
}

void Painter::DrawString(const std::string & code, RECT & rect, UINT flag, Color& color)
{
	::SetBkMode(dc, TRANSPARENT);
	::SetTextColor(dc, color);
	::DrawTextA(dc, code.c_str(), -1, &rect, flag);
}
