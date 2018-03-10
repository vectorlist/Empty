#include <PCH.h>
#include <Renderer/Batch//DX11/DXDebugBatch.h>
#include <Cache/ShaderCache.h>
#include <Graphics/context.h>
#include <Graphics/DX11/DXConfig.h>

DXDebugBatch::DXDebugBatch()
	: vbo(nullptr)
{
	Init();
}

DXDebugBatch::~DXDebugBatch()
{
	SAFE_RELEASE(vbo);
}

void DXDebugBatch::Init()
{
	mShader = ShaderCache::CreateAABBShader();

	//create dynamic buffer
	D3D11_BUFFER_DESC info{};
	info.Usage = D3D11_USAGE_DYNAMIC;
	info.ByteWidth = sizeof(AABBVertex) * MAX_DEBUG_BATCH;
	info.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	info.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	info.StructureByteStride = 0;

	LOG_HR << G_DXDevice->CreateBuffer(&info, nullptr, &vbo);
}

void DXDebugBatch::UpdateAABB()
{
	D3D11_MAPPED_SUBRESOURCE mapped;

	G_DXContext->Map(vbo, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	//AABBVertex* pData = (AABBVertex*)mapped.pData;

	memcpy(mapped.pData, mAABBs.data(), sizeof(AABBVertex) * mAABBs.size());

	G_DXContext->Unmap(vbo, 0);
}


void DXDebugBatch::RenderAABB(const AABB* aabb, const Matrix4x4& world, const vec3f& color)
{
	AABBVertex item;
	item.min = aabb->min;
	item.max = aabb->max;
	item.color = color;
	item.world = world;

	mAABBs.emplace_back(item);
}

void DXDebugBatch::RenderBatch()
{
	if (mAABBs.size() > 0)
	{
		UpdateAABB();
		mShader->Bind();

		uint stride = sizeof(AABBVertex);
		uint offset = 0;
		uint instancedNum = mAABBs.size();
		G_DXContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		G_DXContext->IASetVertexBuffers(0, 1, &vbo, &stride, &offset);
		G_DXContext->DrawInstanced(24, instancedNum, 0, 0);
		mAABBs.clear();
	}

}

