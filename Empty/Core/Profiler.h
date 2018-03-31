#pragma once

#include <chrono>

#define _PROFILE

#ifndef _PROFILE
#define PROFILE_START(x)	(void(0))
#define PROFILE_END(x)		(void(0))
#define PROFILE_MS(x)		0.0
#else
#define PROFILE_START(x)	Profiler::Start(x)
#define PROFILE_END(x)		Profiler::End(x)
#define PROFILE_MS(x)		Profiler::GetMS(x)
#endif

typedef std::chrono::high_resolution_clock				Clock;
typedef std::chrono::high_resolution_clock::time_point	TimePoint;

struct ProfilerSample
{
	ProfilerSample() : ElapseTime(0.0), IsStarted(false) {}
	double			ElapseTime;
	TimePoint		StartTime;
	bool			IsStarted;
};

class Profiler
{
public:
	Profiler(){}

	static void		Start(const std::string& name);
	static void		End(const std::string& name);
	static double	GetMS(const std::string& name);

	static uint frame;
	static std::unordered_map<std::string, ProfilerSample> gSamples;
};


// Sample Start -------------- Sample End (Start - End Counter) to ElapseTime