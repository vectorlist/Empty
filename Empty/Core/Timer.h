#pragma once

#include <Windows.h>

class Timer
{
public:
	Timer();
	~Timer();

	float GetTotalTime() const;
	float GetDeltaTime() const;

	void Reset();
	void Start();
	void Stop();
	void Count();

	void Begin();
	int fps = 0;
	static Timer* globalTimer;
private:
	double mSecondPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPauseTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrentTime;

	bool mStopped;
};

#define GTimer		Timer::globalTimer
