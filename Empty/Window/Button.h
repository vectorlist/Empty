#pragma once

#include <window/window.h>

enum class ButtonType : uint
{
	None,
	Close,
	Min,
	Max
};

class Button : public Window
{
public:
	Button(uint fixedSize, ButtonType type, Window* parent);

protected:
	void PreRegisterClass(WNDCLASS& wc) override;
	void PreCreateStruct(CREATESTRUCT& sc) override;
	LRESULT LocalWndProc(UINT msg, WPARAM wp, LPARAM lp);

	//local paint
	void PaintEvent(Painter* painter);
	void PaintInternal(Painter* painter);

	uint		mFixedSize;
	ButtonType	mType;

	bool mOnEnter = false;
};