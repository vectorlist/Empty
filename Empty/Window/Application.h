#pragma once

#include <Core/Timer.h>
#include <Window/Applicationinfo.h>
#include <Window/Window.h>

#define TITLEBAR_FIXEDSIZE		32

class Scene;
class RenderWindow;
class TitleBar;
class Application : public Window
{
public:
	Application();
	virtual~Application();

protected:
	void PreRegisterClass(WNDCLASS& wc) override;
	void PreCreateStruct(CREATESTRUCT& cs) override;

	virtual LRESULT LocalWndProc(UINT msg, WPARAM wp, LPARAM lp) override;

public:
	int Run();
	const ApplicationInfo& GetInfo() const { return mInfo; };

	bool mIsRunning;

	RenderWindow*	mRenderWindow;
	TitleBar*		mTitleBar;
	Timer			mTimer;
	ApplicationInfo mInfo;
};