#include <Core/PCH.h>
#include <Math/Random.h>
#include <time.h>

std::default_random_engine Random::gRandomEngine;
//std::mt19937 gMtEngine = std::mt19937(time(NULL));
std::uniform_real_distribution<float> Random::gFloatDist = std::uniform_real_distribution<float>(0.0f, 1.0f);

float Random::GenDistFloatZeroToOne()
{
	return gFloatDist(gRandomEngine);
}

float Random::GenDistFloat(float min, float max)
{
	auto dist = std::uniform_real_distribution<float>(min, max);
	return dist(gRandomEngine);
}

float Random::GenMtFloat()
{
	//return gMtEngine();
	return 0.0f;
}
