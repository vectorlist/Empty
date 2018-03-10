#pragma once

#include <math/vec2f.h>

struct MouseState
{
	bool LButton;
	bool RButton;
	vec2i pos;
};

class Input
{
public:
	Input(void* hwnd);
	~Input();

	vec2i GetMousePos() const;
	bool  IsKeyPressed(char c);

	void ProcessInputs();
	vec2i& GetMouseDelta();

	MouseState mouse;
	static Input* globalInput;
private:
	void* hwnd;
	bool firtstPress;
	vec2i mMouseDelta;
	vec2i mLastPos;

};

#define GInput	Input::globalInput

