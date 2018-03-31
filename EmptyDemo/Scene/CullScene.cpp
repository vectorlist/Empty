#include <Core/PCH.h>
#include <Scene/CullScene.h>

#include <Core/Camera.h>
#include <Core/Input.h>
#include <Graphics/Context.h>

#include <Cache/BufferCache.h>

#include <Renderer/Batch/FontBatch.h>
#include <Renderer/Batch/SpriteBatch.h>

#include <Cache/ModelCache.h>
#include <Cache/ShaderCache.h>
#include <Cache/TextureCache.h>
#include <Cache/TerrainCache.h>

#include <Core/Profiler.h>
#include <Renderer/Batch/DebugBatch.h>
#include <Core/Object.h>
#include <Renderer/Environment.h>

uint CullScene::culledCount = 0;

CullScene::~CullScene()
{
}


void CullScene::InitScene()
{
	Viewport v;
	GContext->GetViewport(v);

	mCamera = CreateSceneCamera("Main Camera", 45.f, (float)v.w / (float)v.h, 0.1f, 1000.f);

	BufferCreateInfo info{};
	info.pData = nullptr;
	info.size = sizeof(Matrix4x4) * 2;
	info.type = BufferType::BUFFER_DYNAMIC;
	info.binding = BufferBinding::BIND_UNIFORM;

	mUbo0 = BufferCache::CreateUniformBuffer(&info);
	info.size = sizeof(Matrix4x4);
	mUbo1 = BufferCache::CreateUniformBuffer(&info);


	mModel = ModelCache::LoadModel("../data/model/tree02.obj");

	mModel->mNodes[0].texture = TextureCache::LoadTexture("../data/model/tree00.dds");
	mModel->mNodes[1].texture = TextureCache::LoadTexture("../data/model/tree01.dds");

	mShader = ShaderCache::CreateShader("ForwardVS", "ForwardPS");
	mInstanceShader = ShaderCache::CreateShader("InstanceVS", "ForwardPS");

	int size = 10;
	for (int z = -size; z < size; z++) {
		for (int x = -size; x < size; x++) {

			Object* obj = this->CreateSceneObject();
			obj->mModel = mModel;
			obj->MoveDirection(x * 20, 0, z * 20);
			obj->SetRotation(0, x * 20, z * 30);
			mObject.emplace_back(obj);
			instanceData.emplace_back(obj->GetGlobal());
		}
	}
	
	//for (int x = 0; x < 2; x++) {

	//	Object* obj = this->CreateSceneObject();
	//	obj->mModel = mModel;
	//	obj->MoveDirection(x * 29, 0, 0);
	//	mObject.emplace_back(obj);
	//	instanceData.emplace_back(obj->GetGlobal());
	//}
	

	mCull.BuildSeachNode(&mObject);

	mSky = new SkyCube;
	uint i = 8;
}

void CullScene::UpdateScene(float delta)
{
	mCamera->ProcessFirstPerson(GInput, delta);

	pv[0] = mCamera->GetProjection();
	pv[1] = mCamera->GetView();
	mUbo0->Update(pv);

	mUbo1->Update(model.data());
}

void CullScene::RenderScene()
{
	mUbo0->BindVS(0);
	mUbo1->BindVS(1);

	//mSky->Render();

	Frustum frustum = mCamera->GetExtractedFrustum();

	//mShader->Bind();
	//mModel->Render();

	static void(*DoQuery)(BVHTreeNode*, Frustum&, Matrix4x4*) =
		[](BVHTreeNode* node, Frustum& frustum, Matrix4x4* query)->void
	{
		if (node->DoCulling(frustum)) {
			if (node->IsLeaf()) {
				/*query.emplace_back(node->mObjects[0]->GetGlobal());*/
				query[culledCount] = *(Matrix4x4*)node->customPtr;//node->mObjects[0]->GetGlobal();
				culledCount++;
			}

			if (node->mLeft) {
				DoQuery(node->mLeft, frustum, query);
			}
			if (node->mRight) {
				DoQuery(node->mRight, frustum, query);
			}
		}
	};

	culledCount = 0;
	DoQuery(mCull.mRootNode, frustum, queryData);

	//Sorting Matrix

	GContext->SetBlendState(true);
	GContext->SetCullMode(CullMode::FRONT_AND_BACK);

	mInstanceShader->Bind();
	mModel->RenderInstnaced(culledCount, queryData);

	GContext->SetCullMode(CullMode::BACK);

	//G_DebugBatch->RenderAABB(mCull.mRootNode->GetAABB(), Matrix4x4(), vec3f(1));
	mSky->Render();
}

void CullScene::RenderText()
{

	char buffer[64];

	vec3f pos = mCamera->GetPostion();
	sprintf(buffer, "pos : %.3f %.3f %.3f", pos.x, pos.y, pos.z);
	G_FontBatch->Render(10, 40, buffer, vec4f(1));

	sprintf(buffer, "cull queries : %d", culledCount);
	G_FontBatch->Render(10, 60, buffer, vec4f(1));

	sprintf(buffer, "total instance : %d", instanceData.size());
	G_FontBatch->Render(10, 90, buffer, vec4f(1));

}
