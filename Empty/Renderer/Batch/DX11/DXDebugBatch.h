#pragma once

#include <Renderer/Batch/DebugBatch.h>

#define MAX_DEBUG_BATCH		4096

class ID3D11Buffer;
class DXDebugBatch : public DebugBatch
{
public:
	DXDebugBatch();
	virtual~DXDebugBatch();

	void Init() override;
	void RenderAABB(const AABB* aabb, const Matrix4x4& world, const vec3f& color) override;
	void RenderBatch() override;

	void UpdateAABB();

	Shader* mShader;
	ID3D11Buffer* vbo;
};
