#include <PCH.h>
#include "frustum.h"

//DirectX and Clip Control Zero To One OpenGL NDC

//				  4 ----- 5 ( 1, 1, 1)
//				 /|      /|
//				0 ----- 1 |
//				| 6     | 7
//				| /     |/
//   (-1,-1, 0)	2 ----- 3

vec3f Frustum::NDC[8] = {
	vec3f(-1,  1,  0),
	vec3f( 1,  1,  0),
	vec3f(-1, -1,  0),
	vec3f( 1, -1,  0),

	vec3f(-1,  1,  1),
	vec3f( 1,  1,  1),
	vec3f(-1, -1,  1),
	vec3f( 1, -1,  1)
};

void Frustum::Extract(const Matrix4x4& proj, const Matrix4x4& view)
{
	//Extract with NDC (proj * view)
	Matrix4x4 invMvp = (proj * view).Inverted();

	for (uint i = 0; i < 8; ++i) {
		mVertices[i] = invMvp.TransformCoord(Frustum::NDC[i]);
	}
	//				  4 ----- 5 ( 1, 1, 1)
	//				 /|      /|
	//				0 ----- 1 |
	//				| 6     | 7
	//				| /     |/
	//   (-1,-1, 0)	2 ----- 3
	
	//Counter Clock Wise Cross Order
	//FRONT (0,  1,  2) 
	this->ExtractPlane(mVertices[0], mVertices[1], mVertices[2], &mPlane[PLANE_FRONT]);

	//BACK  (5,  4,  7)
	this->ExtractPlane(mVertices[5], mVertices[4], mVertices[7], &mPlane[PLANE_BACK]);

	//LEFT  (4,  0,  6)
	this->ExtractPlane(mVertices[4], mVertices[0], mVertices[6], &mPlane[PLANE_LEFT]);

	//RIGHT (1,  5,  3)
	this->ExtractPlane(mVertices[1], mVertices[5], mVertices[3], &mPlane[PLANE_RIGHT]);

	//TOP   (4,  5,  0)
	this->ExtractPlane(mVertices[4], mVertices[5], mVertices[0], &mPlane[PLANE_TOP]);

	//BUTTOM(2,  3,  6)
	this->ExtractPlane(mVertices[2], mVertices[3], mVertices[6], &mPlane[PLANE_BOTTOM]);


	//ExtractNoVertice(proj, view);
}

void Frustum::ExtractWithScreenDepth(const Matrix4x4& proj, const Matrix4x4& view, float depth)
{
	//Extract without NDC (proj * view)
	float znear, r;
	Matrix4x4 matrix;
	Matrix4x4 p = proj;
	znear = -p[2][3] / p[2][2];
	r = depth / (depth - znear);
	p[2][2] = r;
	p[2][3] = -r * znear;

	//this is column major order (use view * proj if u row major order)
	matrix = p * view;

	mPlane[PLANE_FRONT][0] = matrix[3][0] + matrix[2][0];
	mPlane[PLANE_FRONT][1] = matrix[3][1] + matrix[2][1];
	mPlane[PLANE_FRONT][2] = matrix[3][2] + matrix[2][2];
	mPlane[PLANE_FRONT][3] = matrix[3][3] + matrix[2][3];
	mPlane[PLANE_FRONT].Normalize();
	
	mPlane[PLANE_BACK][0] = matrix[3][0] - matrix[2][0];
	mPlane[PLANE_BACK][1] = matrix[3][1] - matrix[2][1];
	mPlane[PLANE_BACK][2] = matrix[3][2] - matrix[2][2];
	mPlane[PLANE_BACK][3] = matrix[3][3] - matrix[2][3];
	mPlane[PLANE_BACK].Normalize();

	mPlane[PLANE_LEFT][0] = matrix[3][0] + matrix[0][0];
	mPlane[PLANE_LEFT][1] = matrix[3][1] + matrix[0][1];
	mPlane[PLANE_LEFT][2] = matrix[3][2] + matrix[0][2];
	mPlane[PLANE_LEFT][3] = matrix[3][3] + matrix[0][3];
	mPlane[PLANE_LEFT].Normalize();

	mPlane[PLANE_RIGHT][0] = matrix[3][0] - matrix[0][0];
	mPlane[PLANE_RIGHT][1] = matrix[3][1] - matrix[0][1];
	mPlane[PLANE_RIGHT][2] = matrix[3][2] - matrix[0][2];
	mPlane[PLANE_RIGHT][3] = matrix[3][3] - matrix[0][3];
	mPlane[PLANE_RIGHT].Normalize();

	mPlane[PLANE_TOP][0] = matrix[3][0] - matrix[1][0];
	mPlane[PLANE_TOP][1] = matrix[3][1] - matrix[1][1];
	mPlane[PLANE_TOP][2] = matrix[3][2] - matrix[1][2];
	mPlane[PLANE_TOP][3] = matrix[3][3] - matrix[1][3];
	mPlane[PLANE_TOP].Normalize();

	mPlane[PLANE_BOTTOM][0] = matrix[3][0] + matrix[1][0];
	mPlane[PLANE_BOTTOM][1] = matrix[3][1] + matrix[1][1];
	mPlane[PLANE_BOTTOM][2] = matrix[3][2] + matrix[1][2];
	mPlane[PLANE_BOTTOM][3] = matrix[3][3] + matrix[1][3];
	mPlane[PLANE_BOTTOM].Normalize();
}

void Frustum::ExtractPlane(const vec3f& p0, const vec3f& p1, const vec3f& p2, Plane* dstPlane, bool isInside)
{
	auto N = vec3f::cross((p1 - p0), (p2 - p0)).normalized();
	float distance = vec3f::dot(N, p0);
	if (isInside) {
		distance *= -1.0f;
	}
	dstPlane->n = N;
	dstPlane->d = distance;
	//dstPlane = &dstPlane->Normalized();
}

float Frustum::PlaneDot(const Plane& plane, const vec3f& point)
{
	return vec3f::dot(plane.n, point) + plane.d;
}


//Sphere of AABB testing
bool Frustum::IsInAABBSphere(const vec3f& center, const float radius)
{
	for (int i = 0; i < 6; ++i)
	{
		float distance = PlaneDot(mPlane[i], center);

		if (distance > radius) {
			return false;
		}
	}
	return true;
}

bool Frustum::IsOutAABBSphere(const vec3f& center, const float radius)
{
	for (int i = 0; i < 6; ++i)
	{
		float distance = PlaneDot(mPlane[i], center);

		if (distance < radius) {
			return false;
		}
	}
	return true;
}

bool Frustum::IsOutAABBCenterRadius(const vec3f& center, const float radius)
{
	//more optimize
	for (int i = 0; i < 6; ++i)
	{
		if (PlaneDot(mPlane[i], vec3f(center.x - radius, center.y - radius, center.z - radius)) >= 0.0f)
			continue;
		if (PlaneDot(mPlane[i], vec3f(center.x + radius, center.y - radius, center.z - radius)) >= 0.0f)
			continue;
		if (PlaneDot(mPlane[i], vec3f(center.x - radius, center.y + radius, center.z - radius)) >= 0.0f)
			continue;
		if (PlaneDot(mPlane[i], vec3f(center.x + radius, center.y - radius, center.z - radius)) >= 0.0f)
			continue;
		if (PlaneDot(mPlane[i], vec3f(center.x - radius, center.y - radius, center.z + radius)) >= 0.0f)
			continue;
		if (PlaneDot(mPlane[i], vec3f(center.x + radius, center.y - radius, center.z + radius)) >= 0.0f)
			continue;
		if (PlaneDot(mPlane[i], vec3f(center.x - radius, center.y + radius, center.z + radius)) >= 0.0f)
			continue;
		if (PlaneDot(mPlane[i], vec3f(center.x + radius, center.y + radius, center.z + radius)) >= 0.0f)
			continue;
		return false;
	}
	return true;
}

bool Frustum::IsOutPoint(const vec3f & point)
{
	int i;
	for (i = 0; i < 6; ++i) {
		if (PlaneDot(mPlane[i], point) < 0.0f) {
			return false;
		}
	}
	return true;
}

//out normal is toward inside
//http://old.cescg.org/CESCG-2002/DSykoraJJelinek/ //VFC AABB of SPHERE
bool Frustum::IsInAABB(AABB * aabb)
{
	float m;
	float n;
	vec3f center = aabb->GetCenter();
	int i;
	for (i = 0; i < 6; ++i)
	{ 
		Plane* p = &mPlane[i];

		//sqrt dot
		m = (aabb->max.x * p->n.x) + (aabb->max.y * p->n.y) * (aabb->max.z * p->n.z) + p->d;
		n = (center.x * std::fabsf(p->n.x)) + (center.y * std::fabsf(p->n.y)) +
			(center.z * std::fabsf(p->n.z));

		if (m + n > 0) {
			return false;
		}
		if (m - n > 0) {
			return true;
		}
	}
	return true;
}

bool Frustum::IsInPoint(const vec3f& point)
{
	int i = 0;
	for (i = 0; i < 6; ++i) {
		if (PlaneDot(mPlane[i], point) > 0.0f) {
			return false;
		}
	}
	return true;
}
