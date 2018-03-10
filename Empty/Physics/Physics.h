#pragma once

typedef unsigned int uint;
class Object;
class Physics
{
public:
	static AABB GetTotalAABBFromObjects(Object** pObjecsts, uint size);
	static void GetTransformedAABB(const AABB& a, const Matrix4x4& m, AABB* b);
	static void GetTransformedSphere(const AABB& a, const Matrix4x4& m, vec3f* c, vec3f *he);
};