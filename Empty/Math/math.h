#pragma once

#include <algorithm>
#undef min
#undef max

const float MATRIX_INVERSE_EPSILON = 1e-14f;
const float MATRIX_EPSILON = 1e-5f;

//#define VML_PI 3.14159265358979323846f
#define VML_PI	3.141592654f
#define VML_PI2	6.283185307f

#define RADIANS VML_PI / 180.f
#define DEGREES 180.f / VML_PI

#define FLOAT_EPSILON	0.00001f
#define	FLOAT_EQUAL( a, b )		( abs((a) - (b) ) < FLOAT_EPSILON )
#define	FLOAT_ZERO(a)			( abs((a)) < FLOAT_EPSILON )
#define	VECTOR_ZERO(v)			( FLOAT_ZERO((v).x) && FLOAT_ZERO((v).y) && FLOAT_ZERO((v).z) )
#define INV_LOG2NAT2	1.4426950408889634073599246810019f

namespace math
{
	const inline float Radians(float deg)
	{
		return VML_PI / 180.f * deg;
	}

	const inline float Degree(float rad)
	{
		return 180.f / VML_PI * rad;
	}

	const inline float clamp(float value, float _min = 0.0f, float _max = 1.0f)
	{
		if (value < _min)
			return _min;
		else if (value > _max)
			return _max;

		return value;
	}

	const inline float ClampMinusOneToPlusOne(float value)
	{
		if (value < -1.0f)
			return -1.0f;
		else if (value > 1.0f)
			return 1.0f;

		return value;
	}

	const inline float Normalize(float current, float next, float factor)
	{
		return (factor - current) / (next - current);
	}

	inline float LogF2(float x)
	{
		return logf(x) * INV_LOG2NAT2;
	}

}

//if (isMip)
//{
//	const uint32_t max = max(max(_width, _height), _depth);
//	const uint32_t num = 1 + uint32_t(LogF2(float(max)));
//
//	return uint8_t(num);
//}
//
//return 1;