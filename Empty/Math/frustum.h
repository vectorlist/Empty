#pragma once

#include <math/matrix4x4.h>
#include <math/vec4f.h>

struct Plane
{
	vec3f n;
	float d;
	Plane&	Normalize();
	Plane	Normalized();
	__forceinline float& operator[](unsigned int index) {
		assert(index >= 0 && index <= 3);
		return (&n.x)[index];
	}
	__forceinline float operator[](unsigned int index) const {
		assert(index >= 0 && index <= 3);
		return (&n.x)[index];
	}
};

//normalize Itself
inline Plane& Plane::Normalize()
{
	float len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
	float invLen = 1.0f / len;
	this->n = this->n * invLen;
	this->d = this->d * invLen;
	return *this;
}

inline Plane Plane::Normalized()
{
	float len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
	float invLen = 1.0f / len;
	Plane plane;
	plane.n = this->n * invLen;
	plane.d = this->d * invLen;
	return plane;
}

#define PLANE_FRONT		0U
#define PLANE_BACK		1U
#define PLANE_LEFT		2U
#define PLANE_RIGHT		3U
#define PLANE_TOP		4U
#define PLANE_BOTTOM	5U
#define PLANE_MAX		6U

struct Frustum
{
	void Extract(const Matrix4x4& proj, const Matrix4x4& view);
	void ExtractWithScreenDepth(const Matrix4x4& proj, const Matrix4x4& view, float depth);
	//Plane N to Inside ot outside
	void ExtractPlane(const vec3f& p0, const vec3f& p1, const vec3f& p2, Plane* dstPlane, bool isInside = true);



	Plane mPlane[PLANE_MAX];
	vec3f mVertices[8];		

	static vec3f NDC[8];

	float PlaneDot(const Plane& plane, const vec3f& point);

	//in bound normal mothod
	bool IsInAABBSphere(const vec3f& center, const float radius);
	bool IsInAABB(AABB* aabb);
	bool IsInPoint(const vec3f& point);

	//outside method
	bool IsOutAABBSphere(const vec3f& center, const float radius);
	bool IsOutAABBCenterRadius(const vec3f& center, const float radius);
	bool IsOutPoint(const vec3f& point);
};

//emonstrates the fact that the conversion of the AABB to the bounding 
//sphere is not exact. If we measure the entire radius of the sphere 
//and the distance from the center of the AABB we can see that we only need to compare its projection to the direction of the normal vector of the tested plane. There is one more dot product compared with the simple sphere-plane test. The following 
//pseudo-code describes more implementation details of this method:


//int AABBvsFrustum(AABB *b, FRUSTUM *f)
//{
//	float m, n; int i, result = INSIDE;
//
//	for (i = 0; i < 6; i++) {
//		PLANE *p = f->plane + i;
//
//		m = (b->mx * p->a) + (b->my * p->b) + (b->mz * p->c) + p->d;
//		n = (b->dx * fabs(p->a)) + (b->dy * fabs(p->b)) + (b->dz * fabs(p->c));
//
//		if (m + n < 0) return OUTSIDE;
//		if (m - n < 0) result = INTERSECT;
//
//	} return result;
//}

// f(n) = f(n/2) + 1 I get f(n/2^k) + k

// when n = 2k calim that f(2k) = k + 1 for k > this is true f(2) hold assume as i know lol

// 