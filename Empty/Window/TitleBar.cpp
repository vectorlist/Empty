#include <Core/PCH.h>
#include <window/titlebar.h>
#include <window/button.h>
#include <window/applicationinfo.h>

TitleBar::TitleBar(uint x, uint y, uint w, uint h, Window * parent)
	: Window(x, y, w, h, parent)
{
	Create();
}

TitleBar::~TitleBar()
{
	SAFE_DELETE(mCloseButton);
}

void TitleBar::PreRegisterClass(WNDCLASS & wc)
{
	wc.lpszClassName = "titlebar";
	wc.hbrBackground = CreateSolidBrush(TITLEBAR_COLOR);
}

void TitleBar::PreCreateStruct(CREATESTRUCT &cs)
{
	//auto visible if we have parent
	if (mParent) {
		cs.style |= WS_VISIBLE;
	}
}

LRESULT TitleBar::LocalWndProc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		//TODO : max button min button
		mCloseButton = new Button(height, ButtonType::Close, this);
		break;
	case WM_SIZE:
	{
		int w = LOWORD(lp);
		int h = HIWORD(lp);
		break;
	}
	case WM_PAINT:
	{
		auto info = GetSubSystem<ApplicationInfo>();
		Painter painter(mHandle);
		static HFont font(FONT_SEGOEUI_FAMILY, 20);
		painter.SetFont(font);
		RECT rect = GetRect();
		rect.left += 12;
		painter.DrawString(info->appName + " " + info->version + " " + info->GetApiString(),
			 rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER , Color(230));
		break;
	}
	case WM_NCHITTEST:
	{
		return HTTRANSPARENT;
	}
	}
	return DefWindowProc(mHandle, msg, wp, lp);
}
