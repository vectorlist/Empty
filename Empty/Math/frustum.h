#pragma once

#include <math/matrix4x4.h>
#include <math/vec4f.h>

struct Plane
{
	float x, y, z, d;
	Plane Normalized();
};

inline Plane Plane::Normalized()
{
	float len = sqrtf(x * x + y * y + z * z);
	float invLen = 1.0f / len;
	
	this->x = x * invLen;
	this->y = y * invLen;
	this->z = z * invLen;
	this->d = d * invLen;
}

struct Frustum
{
	enum {};
	//TODO Extract Planes(view matrix, proj matrix)

	void ExtractPlanes(const Matrix4x4& proj, const Matrix4x4& view);

	vec4f mPlanes[6];
};


// Now that we have our modelview and projection matrix, if we combine these 2 matrices,
// it will give us our clipping planes.  To combine 2 matrices, we multiply them.

//clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12];
//clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13];
//clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14];
//clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];
//
//clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12];
//clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13];
//clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14];
//clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];
//
//clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12];
//clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13];
//clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14];
//clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15];
//
//clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12];
//clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13];
//clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14];
//clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15];

//// This will extract the RIGHT side of the frustum
//m_Frustum[RIGHT][A] = clip[3] - clip[0];
//m_Frustum[RIGHT][B] = clip[7] - clip[4];
//m_Frustum[RIGHT][C] = clip[11] - clip[8];
//m_Frustum[RIGHT][D] = clip[15] - clip[12];
//
//// Now that we have a normal (A,B,C) and a distance (D) to the plane,
//// we want to normalize that normal and distance.
//
//// Normalize the RIGHT side
//NormalizePlane(m_Frustum, RIGHT);
//
//// This will extract the LEFT side of the frustum
//m_Frustum[LEFT][A] = clip[3] + clip[0];
//m_Frustum[LEFT][B] = clip[7] + clip[4];
//m_Frustum[LEFT][C] = clip[11] + clip[8];
//m_Frustum[LEFT][D] = clip[15] + clip[12];