#pragma once

#include <window/window.h>

class Button;
class TitleBar : public Window
{
public:
	TitleBar(uint x, uint y, uint w, uint h, Window* parent);
	virtual~TitleBar();

protected:
	void PreRegisterClass(WNDCLASS& wc) override;
	void PreCreateStruct(CREATESTRUCT& cs) override;

	LRESULT LocalWndProc(UINT msg, WPARAM wp, LPARAM lp);

	Button* mCloseButton;
};