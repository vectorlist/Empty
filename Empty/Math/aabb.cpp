#include <PCH.h>
#include <math/aabb.h>

//======================== SPHERE =================================
BSpehre::BSpehre(const vec3f& min, const vec3f& max)
{
	//local contructor
	mCenter = (min + max) * 0.5;
	mHalf = (max * min) * 0.5;
	mRadius = (mCenter - min).length();
}

//GET WORLD CAENTER, RADIUS FROM LOCAL
void BSpehre::GetWorldCenterRadius(const Matrix4x4& world, vec3f * pCenter, float * pRadius)
{
	*pCenter = world * mCenter;
	*pRadius = mHalf.length();
}


//======================== AABB ====================================
//INIT AABB AND SPHERE 
AABB::AABB(const vec3f& min, const vec3f& max)
	: BSpehre(min, max), min(min), max(max)
{
}

AABB  AABB::GetTransformedAABB(const Matrix4x4& world)
{
	//http://dev.theomader.com/transform-bounding-boxes/
	vec3f    right(world[0][0], world[1][0], world[2][0]);
	vec3f		up(world[0][1], world[1][1], world[2][1]);
	vec3f  forawrd(world[0][2], world[1][2], world[2][2]);

	vec3f xa = right * min.x;
	vec3f xb = right * max.x;

	vec3f ya = up * min.y;
	vec3f yb = up * max.y;

	vec3f za = forawrd * min.z;
	vec3f zb = forawrd * max.z;

	vec3f pos(world[0][3], world[1][3], world[2][3]);

	//expensive
	vec3f newMin = vec3f::min(xa, xb) + vec3f::min(ya, yb) + vec3f::min(za, zb) + pos;
	vec3f newMax = vec3f::max(xa, xb) + vec3f::max(ya, yb) + vec3f::max(za, zb) + pos;

	return AABB(newMin, newMax);
}


