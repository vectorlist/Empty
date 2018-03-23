#pragma once

#include <math/matrix4x4.h>

struct BSpehre
{
	BSpehre(): mRadius(0.0f){}
	BSpehre(const vec3f& min, const vec3f& max);
	vec3f mCenter;
	float mRadius;
	vec3f mHalf;
	vec3f&	GetCenter()			{ return mCenter; }
	float	GetRadius() const	{ return mRadius; }
	void	GetWorldCenterRadius(const Matrix4x4& world, vec3f* pCenter, float* pRadius);
};

struct AABB : public BSpehre
{
	enum Axis { X = 0U, Y = 1U, Z = 2U};
	AABB() : min(FLT_MAX), max(-FLT_MAX) {}
	AABB(const vec3f& min, const vec3f& max);

	void    Init(const vec3f& min, const vec3f& max);
	void	Expand(const AABB& aabb);
	void	Expand(const vec3f& point);
	Axis	GetLongestAxis() const;
	AABB    GetTransformedAABB(const Matrix4x4& world);
	float	GetValueFromAxis(Axis axis);
	vec3f   GetMidPoint() const;
	float	GetRelativeRadius() const;
	vec3f   GetSize() const;
	
	vec3f	min;
	vec3f	max;

	//test
	vec3f	mVertices[8];
	bool	IsInsidePoint(const vec3f& p);
	vec3f   GetVertex(uint index) const;
};

inline void AABB::Init(const vec3f &min, const vec3f& max)
{
	//local contructor
	this->min = min;
	this->max = max;
	mCenter = (min + max) * 0.5;
	mHalf = (max * min) * 0.5;
	mRadius = (mCenter - min).length();
}

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

//Get relative Center of box
inline vec3f AABB::GetMidPoint() const
{
	return (min + max) * 0.5f;
}

//Get Relative Radius
inline float AABB::GetRelativeRadius() const
{
	return ((max - min) * 0.5f).length();
}

inline vec3f AABB::GetSize() const
{
	return max - min;
}

inline bool AABB::IsInsidePoint(const vec3f& p)
{
	if (p.x < min.x) return false;
	if (p.y < min.y) return false;
	if (p.z < min.z) return false;
	if (p.x > max.x) return false;
	if (p.y > max.y) return false;
	if (p.z > max.z) return false;
	return true;
}

inline vec3f AABB::GetVertex(uint index) const
{
	vec3f temp;
	temp.x = (index & (1 << 0)) ? max[0] : min[0];
	temp.y = (index & (1 << 1)) ? max[1] : min[1];
	temp.z = (index & (1 << 2)) ? max[2] : min[2];
	return temp;
}

