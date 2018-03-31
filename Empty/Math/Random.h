#pragma once

#include <random>


class Random
{
public:
	static float GenDistFloatZeroToOne();
	static float GenDistFloat(float min, float max);
	static float GenMtFloat();

	//Engine
	static std::default_random_engine	gRandomEngine;
	//static std::mt19937					gMtEngine;

	//Distribution
	static std::uniform_real_distribution<float> gFloatDist;

};