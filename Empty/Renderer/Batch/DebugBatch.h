#pragma once

#define DRAW_ORDER_AABB_COUNT		24

struct AABBVertex
{
	vec3f		min;
	vec3f		max;
	vec3f		color;
	Matrix4x4	world;
};

class DebugBatch
{
public:
	DebugBatch();

	virtual void RenderAABB(const AABB* aabb, const Matrix4x4& world, const vec3f& color) = 0;
	virtual void RenderBatch() = 0;
	static DebugBatch* GetInstance() { return globalDebugBatch; }
protected:
	virtual void Init() = 0;
	static DebugBatch* globalDebugBatch;
	std::vector<AABBVertex> mAABBs;
};

#define G_DebugBatch	DebugBatch::GetInstance()
