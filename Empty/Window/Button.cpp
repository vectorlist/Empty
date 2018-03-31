#include <Core/PCH.h>
#include <window/button.h>

Button::Button(uint fixedSize, ButtonType type, Window *parent)
	: Window(parent), mFixedSize(fixedSize), mType(type)
{
	Create();
}

void Button::PreRegisterClass(WNDCLASS& wc)
{
	//make sure unique class name for this class
	wc.lpszClassName = "window button";
	//test color
	wc.hbrBackground = CreateSolidBrush(RGB(100, 0, 0));
}

void Button::PreCreateStruct(CREATESTRUCT& sc)
{
	//-------------------- title bar ----------------------- width
	uint w = mParent->width;
	//set x,y
	sc.x = w - mFixedSize;
	sc.y = 0;
	sc.cx = mFixedSize;		//width
	sc.cy = mFixedSize;		//height

	//reset width and height
	width = sc.cx;
	height = sc.cy;

	//add auto visible
	sc.style |= WS_VISIBLE;
}

void Button::PaintEvent(Painter* painter)
{
	RECT rect;
	GetClientRect(mHandle, &rect);

	static HBRUSH brush = CreateSolidBrush(BUTTON_COLOR);
	static HBRUSH onBrush = CreateSolidBrush(BUTTON_LCOLOR);

	if (!mOnEnter)
		::FillRect(painter->dc, &rect, brush);
	else
		::FillRect(painter->dc, &rect, onBrush);

	PaintInternal(painter);
}

void Button::PaintInternal(Painter* painter)
{
	static Pen pen(PS_SOLID, 2, Color(200, 200, 200));

	painter->SetPen(pen);

	int space = 11;

	if (mType == ButtonType::Close) {
		Point a(space, space);
		Point b(width - space, height - space);

		Point c(space, height - space);
		Point d(width - space, space);

		painter->DrawLine(a, b);
		painter->DrawLine(c, d);
	}
}


LRESULT Button::LocalWndProc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		Painter painter(mHandle);
		PaintEvent(&painter);
		break;
	}
	case WM_USER_ENTEREVENT:
		auto rect = GetRect();
		::InvalidateRect(mHandle, &rect, true);
		break;
	case WM_MOUSEHOVER:
	{
		if (!mOnEnter) {
			mOnEnter = true;
			PostMessage(mHandle, WM_USER_ENTEREVENT, 0, 0);
		}
		break;
	}
	case WM_MOUSELEAVE:
	{
		if (mOnEnter) mOnEnter = false;
		//for repaint
		auto rect = GetRect();
		::InvalidateRect(mHandle, &rect, true);
		break;
	}
	case WM_MOUSEMOVE:
	{
		TRACKMOUSEEVENT tm{};
		tm.cbSize = sizeof(TRACKMOUSEEVENT);
		tm.dwFlags = TME_HOVER | TME_LEAVE;		//we allow to access hover and leave event
		tm.hwndTrack = mHandle;
		tm.dwHoverTime = 1;
		TrackMouseEvent(&tm);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		LOG << "clock" << ENDN;
		if (mType == ButtonType::Close) {
			LOG << "inner clock" << ENDN;
			PostQuitMessage(0);
		}
		break;
	}
	}

	return DefWindowProc(mHandle, msg, wp, lp);
}

