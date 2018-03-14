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
	
	vec3f	min;
	vec3f	max;

	//test
	vec3f	mVertices[8];
	void	SetVertices(const vec3f& min, const vec3f& max);
	bool	IsInsidePoint(const vec3f& p);
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

inline void AABB::SetVertices(const vec3f& min, const vec3f& max)
{
	mVertices[0] = vec3f(min.x, min.y, min.z);
	mVertices[1] = vec3f(max.x, min.y, min.z);
	mVertices[2] = vec3f(min.x, max.y, min.z);
	mVertices[3] = vec3f(max.x, max.y, min.z);
	mVertices[4] = vec3f(min.x, min.y, max.z);
	mVertices[5] = vec3f(max.x, min.y, max.z);
	mVertices[6] = vec3f(min.x, max.y, max.z);
	mVertices[7] = vec3f(max.x, max.y, max.z);
}

inline bool AABB::IsInsidePoint(const vec3f& p)
{
	if (p.x < mVertices[0].x) return false;
	if (p.y < mVertices[0].y) return false;
	if (p.z < mVertices[0].z) return false;
	if (p.x > mVertices[7].x) return false;
	if (p.y > mVertices[7].y) return false;
	if (p.z > mVertices[7].z) return false;
	return true;
}

