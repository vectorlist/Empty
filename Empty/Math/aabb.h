#pragma once

#include <math/matrix4x4.h>

struct BSpehre
{
	BSpehre(): mRadius(0.0f){}
	BSpehre(const vec3f& min, const vec3f& max);
	vec3f mCenter;
	float mRadius;
	vec3f mHalf;

	void GetWorldCenterRadius(const Matrix4x4& world, vec3f* pCenter, float* pRadius);
};

struct AABB : public BSpehre
{
	enum Axis { X = 0U, Y = 1U, Z = 2U};
	AABB() : min(FLT_MAX), max(-FLT_MAX) {}
	AABB(const vec3f& min, const vec3f& max);

	void	Expand(const AABB& aabb);
	void	Expand(const vec3f& point);
	Axis	GetLongestAxis() const;
	AABB    GetTransformedAABB(const Matrix4x4& world);
	float	GetValueFromAxis(Axis axis);
	vec3f   GetMidPoint() const;
	float	GetRadius() const;
	
	vec3f min;
	vec3f max;
};

inline void AABB::Expand(const AABB& aabb)
{
	min = vec3f::min(min, aabb.min);
	max = vec3f::max(max, aabb.max);
}

inline void AABB::Expand(const vec3f & point)
{
	min = vec3f::min(min, point);
	max = vec3f::min(max, point);
}

inline AABB::Axis AABB::GetLongestAxis() const
{
	vec3f P = max - min;
	if ((P.x > P.y) && (P.x > P.z)) {
		return AABB::X;
	}
	if ((P.y > P.x) && (P.y > P.z)) {
		return AABB::Y;
	}
	return AABB::Z;
}

inline float AABB::GetValueFromAxis(Axis axis)
{
	vec3f P = max - min;
	return P[axis];
}

inline vec3f AABB::GetMidPoint() const
{
	return (min + max) * 0.5f;
}

inline float AABB::GetRadius() const
{
	return ((max - min) * 0.5f).length();
}

