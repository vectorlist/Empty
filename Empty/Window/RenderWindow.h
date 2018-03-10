#pragma once

#include <renderer/renderer.h>
#include <window/window.h>
#include <memory>


class RenderWindow : public Window
{
public:
	RenderWindow(uint x, uint y,uint width, uint height, Window* parent = NULL);

	void		CreateRenderer();
	Renderer*	GetRenderer();
protected:
	void PreRegisterClass(WNDCLASS &wc);
	void PreCreateStruct(CREATESTRUCT &cs);
	LRESULT LocalWndProc(UINT msg, WPARAM wp, LPARAM lp) override;

private:
	Renderer mRenderer;
};