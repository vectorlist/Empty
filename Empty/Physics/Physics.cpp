#include <PCH.h>
#include <Physics/Physics.h>
#include <Core/Object.h>

AABB Physics::GetTotalAABBFromObjects(Object** pObjecsts, uint size)
{
	AABB aabb;
	if (size < 1) return aabb;

	aabb = pObjecsts[0]->GetWorldAABB();

	for (uint i = 1; i < size; ++i)
	{
		aabb.Expand(pObjecsts[i]->GetWorldAABB());
	}
	return aabb;
}

//Christer ericson - real time collision detection pg.86
void Physics::GetTransformedAABB(const AABB& a, const Matrix4x4& m, AABB* b)
{
	for (int i = 0; i < 3; ++i)
	{
		//add translate
		b->min[i] = b->max[i] = m[i][3];
		for (int j = 0; j < 3; j++)
		{
			float e = m[i][j] * a.min[j];
			float f = m[i][j] * a.max[j];
			if (e < f) {
				b->min[i] += e;
				b->max[i] += f;
			}
			else {
				b->min[i] += f;
				b->max[i] += e;
			}
		}
	}
}

//Christer ericson - real time collision detection pg.87
void Physics::GetTransformedSphere(const AABB& a, const Matrix4x4& m, vec3f* c, vec3f* he)
{
	//center and half extend
	for (int i = 0; i < 3; ++i)
	{
		(*c)[i] = m[i][3];
		(*he)[i] = 0.0f;
		for (int j = 0; j < 3; ++j)
		{
			(*c)[i] += m[i][j] * a.mCenter[j];
			(*he)[i] += fabsf(m[i][j]) * a.mHalf[j];
		}
	}
}
