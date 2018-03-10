#pragma once

#include <Windows.h>
#include <memory>

#define SAFE_GDI_DELETE(x) if(x) { ::DeleteObject(x); x = NULL;}
#define SAFE_WIN_DELETE(x) if(x) { ::DestroyWindow(x); x = NULL;}

class Color
{
public:
	Color(int r) { mColor = (BYTE(r)) | (BYTE(r) << 8) | (BYTE(r) << 16); };
	Color() { mColor = (BYTE(0)) | (BYTE(0) << 8) | (BYTE(0) << 16); };
	Color(int r, int g, int b) { mColor = (BYTE(r)) | (BYTE(g) << 8) | (BYTE(b) << 16); };

	//overlap operator using itself to COLORREF
	operator COLORREF() { return mColor; }
	operator COLORREF() const { return mColor; }
	COLORREF mColor;
};

class Point : public POINT
{
public:
	Point() { x = 0; y = 0; }
	Point(int x, int y) { this->x = x; this->y = y; }
	Point(POINT other) { x = other.x; y = other.y; }

	//we can coversation btw Point POINT
	operator POINT() { return *this; }
	operator LPPOINT() { return this; }
};

class GDIObj
{
public:
	GDIObj() {};
	~GDIObj() { SAFE_GDI_DELETE(mData); };

	HGDIOBJ mData;
};

class Pen
{
public:
	Pen()
	{
		mPen = std::make_shared<GDIObj>();
		mPen->mData = CreatePen(PS_SOLID, 1, Color(200,200,200));
	}

	Pen(Color color)
	{
		mPen = std::make_shared<GDIObj>();
		mPen->mData = CreatePen(PS_SOLID, 1, color);
	}
	Pen(int style, int width, const Color& color)
	{
		mPen = std::make_shared<GDIObj>();
		mPen->mData = CreatePen(style, width, color);
	}

	operator HPEN() const { return (HPEN)mPen->mData; }
private:
	std::shared_ptr<GDIObj> mPen;
};

class Brush
{
public:
	Brush()
	{
		mBrush = std::make_shared<GDIObj>();
		mColor = RGB(0,0,0);
		mBrush->mData = CreateSolidBrush(mColor);
	}
	Brush(COLORREF color)
	{
		mBrush = std::make_shared<GDIObj>();
		mColor = color;
		mBrush->mData = CreateSolidBrush(color);
	}

	//return itself to HBRUSH
	COLORREF Color() const { return mColor; }
	operator HBRUSH() const { return (HBRUSH)mBrush->mData; }

private:
	std::shared_ptr<GDIObj> mBrush;
	COLORREF mColor;
};

#define FONT_DEFAULT_SIZE		18
#define FONT_SEGOEUI_FAMILY		"Segoe UI"
#define FONT_CONSOLAS_FAMILY	"Consolas"

class Font
{
public:
	Font()
	{
		mFont = std::make_shared<GDIObj>();
		mFont->mData = CreateFontA(FONT_DEFAULT_SIZE, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, FONT_SEGOEUI_FAMILY);
	}
	Font(LPCSTR family, int size = 20)
	{
		mFont = std::make_shared<GDIObj>();
		mFont->mData = CreateFontA(size, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, family);
	}

	operator HFONT() const { return (HFONT)mFont->mData; }

private:
	std::shared_ptr<GDIObj> mFont;
};