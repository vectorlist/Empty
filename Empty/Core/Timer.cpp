#include <Core/PCH.h>
#include <Core/timer.h>

Timer* Timer::globalTimer = nullptr;

Timer::Timer()
	:
	mDeltaTime(0.0),
	mBaseTime(0),
	mPauseTime(0),
	mPrevTime(0),
	mCurrentTime(0),
	mStopped(false)
{
	__int64 countPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countPerSec);
	mSecondPerCount = 1.0 / (double)countPerSec;
	this->globalTimer = this;
}

Timer::~Timer()
{
}

float Timer::GetTotalTime() const
{
	if (mStopped)
	{
		return (float)(((mStopTime - mPauseTime) - mBaseTime) * mSecondPerCount);
	}
	return (float)(((mCurrentTime - mPauseTime) - mBaseTime) * mSecondPerCount);
}

float Timer::GetDeltaTime() const
{
	return (float)mDeltaTime;
}

void Timer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void Timer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (mStopped)
	{
		mPauseTime += (startTime - mStopTime);
		mStopTime = 0;
		mStopped = false;
	}
}

void Timer::Stop()
{
	if (!mStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopTime = currTime;
		mStopped = true;
	}
}

void Timer::Count()
{
	if (mStopped)
	{
		mDeltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrentTime = currTime;

	mDeltaTime = (mCurrentTime - mPrevTime) * mSecondPerCount;
	mPrevTime = mCurrentTime;

	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}

void Timer::Begin()
{
	static bool started = true;
	if (started)
	{
		Start();
		started = false;
	}

	Count();

	static int frameCount = 0;
	static float timeElapsed = 0.0f;
	frameCount++;

	if (GetTotalTime() - timeElapsed >= 1.0f)
	{
		fps = frameCount;
		frameCount = 0;
		timeElapsed += 1.0f;
		LOG << fps << " dt " << GetDeltaTime() << ENDN;
	}
}
