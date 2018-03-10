#include <PCH.h>
#include <Renderer/batch/GL4/GLDebugBatch.h>
#include <Cache/ShaderCache.h>
#include <Graphics/GL4/GLConfig.h>

GLDebugBatch::GLDebugBatch()
	: vao(0), vbo(0)
{
	Init();
}

GLDebugBatch::~GLDebugBatch()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void GLDebugBatch::Init()
{
	//GL ONLY 
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AABBVertex),
		(void*)offsetof(AABBVertex, AABBVertex::min));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AABBVertex),
		(void*)offsetof(AABBVertex, AABBVertex::max));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(AABBVertex),
		(void*)offsetof(AABBVertex, AABBVertex::color));

	//per one draw instanced
	glVertexAttribDivisor(0, 1);   //min per one draw
	glVertexAttribDivisor(1, 1);   //max per one draw
	glVertexAttribDivisor(2, 1);   //color per one draw

								   //ADD Matrix per Instance
	glEnableVertexAttribArray(3);			//glsl can't pass float4x4 in layout(so we need devide)
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	//offset of [4][4][4][4]
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(AABBVertex),
		(void*)(offsetof(AABBVertex, AABBVertex::world) + (sizeof(vec4f) * 0)));

	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(AABBVertex),
		(void*)(offsetof(AABBVertex, AABBVertex::world) + (sizeof(vec4f) * 1)));

	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(AABBVertex),
		(void*)(offsetof(AABBVertex, AABBVertex::world) + (sizeof(vec4f) * 2)));

	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(AABBVertex),
		(void*)(offsetof(AABBVertex, AABBVertex::world) + (sizeof(vec4f) * 3)));

	//per vertex instance
	glVertexAttribDivisor(3, 1);   //min per one draw
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	mShader = ShaderCache::CreateAABBShader();

}

void GLDebugBatch::UpdateAABBs()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(AABBVertex) * mAABBs.size(), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(AABBVertex) * mAABBs.size(), mAABBs.data());

}

void GLDebugBatch::RenderAABB(const AABB* aabb, const Matrix4x4& world, const vec3f& color)
{
	AABBVertex item;
	item.min = aabb->min;
	item.max = aabb->max;
	item.color = color;
	item.world = world;

	mAABBs.emplace_back(item);
}

void GLDebugBatch::RenderBatch()
{
	//im not sure is it safe
	if (mAABBs.size() > 0)
	{
		UpdateAABBs();
		mShader->Bind();

		uint instancedAABBNum = mAABBs.size();
		glBindVertexArray(vao);
		glDrawArraysInstanced(GL_LINES, 0, DRAW_ORDER_AABB_COUNT, instancedAABBNum);
		glBindVertexArray(0);

		mAABBs.clear();
	}



}

