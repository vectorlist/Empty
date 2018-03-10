#pragma once

#include <Renderer/Batch/DebugBatch.h>

class GLDebugBatch : public DebugBatch
{
public:
	GLDebugBatch();
	virtual~GLDebugBatch();
	void Init();
	void RenderAABB(const AABB* aabb, const Matrix4x4& world, const vec3f& color);
	void RenderBatch();

private:
	GLuint vao;
	GLuint vbo;

	Shader* mShader;
	void UpdateAABBs();

};

