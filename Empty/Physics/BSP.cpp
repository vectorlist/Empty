#include <PCH.h>
#include <Physics/BSP.h>
//#include <Graphics/GL4/GLConfig.h>

#include <Cache/BufferCache.h>

uint BSPTreeNode::counter = 0;

BSPTreeNode::BSPTreeNode()
	: mIndicesCount(0)
{
	counter++;
	mChild[0] = nullptr;
	mChild[1] = nullptr;
}

BSPTreeNode::~BSPTreeNode()
{
	SAFE_DELETE(mChild[0]);
	SAFE_DELETE(mChild[1]);
}

void BSPTreeNode::Init(const vec3f& min, const vec3f& max, uint level, float minaabbSize)
{
	this->mLevel = level;

	//must be init AABB or use Constructor AABB(min, max)
	mAABB.Init(min, max);

	vec3f mid = mAABB.GetMidPoint();
	vec3f size = mAABB.GetSize();

	//For Indices Buffer minimun Level Node (visible indices for 16.f min)
	if (size.x > minaabbSize || size.z > minaabbSize)
	{
		mChild[0] = new BSPTreeNode;
		mChild[1] = new BSPTreeNode;

		//priority x 
		if (size.x >= size.z) {
			mChild[0]->Init(vec3f(min.x, min.y, min.z), vec3f(mid.x, max.y, max.z), level + 1, minaabbSize);
			mChild[1]->Init(vec3f(mid.x, min.y, min.z), vec3f(max.x, max.y, max.z), level + 1, minaabbSize);
		}
		else {
			mChild[0]->Init(vec3f(min.x, min.y, min.z), vec3f(max.x, max.y, mid.z), level + 1, minaabbSize);
			mChild[1]->Init(vec3f(min.x, min.y, mid.z), vec3f(max.x, max.y, max.z), level + 1, minaabbSize);
		}
	}
}

bool BSPTreeNode::ReserveIndices(NodeVertex* vertice, uint* indices, uint p0, uint p1, uint p2)
{
	if (!mAABB.IsInsidePoint(vertice[indices[p0]].pos)) return false;
	if (!mAABB.IsInsidePoint(vertice[indices[p1]].pos)) return false;
	if (!mAABB.IsInsidePoint(vertice[indices[p1]].pos)) return false;
	
	bool attachAbleChild = false;
	if (mChild[0] != nullptr) {
		attachAbleChild != mChild[0]->ReserveIndices(vertice, indices, p0, p1, p2);
	}
	if (mChild[1] != nullptr && attachAbleChild == false) {
		attachAbleChild != mChild[1]->ReserveIndices(vertice, indices, p0, p1, p2);
	}
	if (attachAbleChild == false && IsLeaf()) {
		mIndicesCount += 3;
		mIndices.emplace_back(indices[p0]);
		mIndices.emplace_back(indices[p1]);
		mIndices.emplace_back(indices[p2]);
	}
	return true;
}

uint BSPTreeNode::SumitIndexBufferToGPU()
{
	uint totalIndexedNodes = 0;

	//This Node has a Indices?
	if (mIndicesCount > 0 && mIndices.size() > 0)
	{
		IndexBufferCreateInfo info{};
		info.pIndices = mIndices.data();
		info.IndicesSize = sizeof(uint) * mIndices.size();

		mIndexBuffer = BufferCache::CreateIndexBuffer(&info);

		mIndices.clear();
		totalIndexedNodes++;
	}

	if (mChild[0]) {
		totalIndexedNodes += mChild[0]->SumitIndexBufferToGPU();
	}

	if (mChild[1]) {
		totalIndexedNodes += mChild[1]->SumitIndexBufferToGPU();
	}

	return totalIndexedNodes;
}

//TODO : render per node on terrain level
void BSPTreeNode::Render()
{
	mIndexBuffer->RenderIndexed(Topology::TRIANGLE, mIndicesCount);
}

void BSPTreeNode::ResetAABB(NodeVertex* vertices)
{
	//TODO : reset all of AABB 
	float minY = mAABB.min.y;
	float maxY = mAABB.max.y;

	mAABB.min.y = maxY;
	mAABB.max.y = minY;

	if (mIndicesCount > 0)
	{
		for (int i = 0; i < mIndicesCount; ++i)
		{
			if (vertices[mIndices[i]].pos.y < mAABB.min.y) {
				mAABB.min.y = vertices[mIndices[i]].pos.y;
			}
			if (vertices[mIndices[i]].pos.y > mAABB.max.y) {
				mAABB.max.y = vertices[mIndices[i]].pos.y;
			}
		}
	}

	for (int i = 0; i < 2; ++i) {
		if (mChild[i]) {
			mChild[i]->ResetAABB(vertices);

			if (mChild[i]->mAABB.min.y < mAABB.min.y) mAABB.min.y = mChild[i]->mAABB.min.y;
			if (mChild[i]->mAABB.max.y > mAABB.max.y) mAABB.max.y = mChild[i]->mAABB.max.y;
		}
	}

	mAABB.Init(mAABB.min, mAABB.max);
}

uint BSPTreeNode::QueryVisibility(Frustum& frustum, TreeNode** queryNodes, uint& visibleCount)
{
	int renderVerticesCount = 0;
	mIsVisible = frustum.IsInAABBSphere(mAABB.GetCenter(), mAABB.GetRadius());

	if (mIsVisible) {
		if (mIndicesCount > 0) {
			queryNodes[visibleCount] = this;
			//increase visble count
			visibleCount++;
			//increase how many rendered vertices
			renderVerticesCount += mIndicesCount;
		}
		//has left child ?
		if (mChild[0]) {
			//Recursive
			renderVerticesCount += mChild[0]->QueryVisibility(frustum, queryNodes, visibleCount);
		}
		//has right child
		if (mChild[1]) {
			renderVerticesCount += mChild[1]->QueryVisibility(frustum, queryNodes, visibleCount);
		}
	}
	return renderVerticesCount;
}

//==================================== TREE ========================================

#include <Cache/ShaderCache.h>

BSPTree::BSPTree()
{
}

void BSPTree::Init(NodeVertex* vertices, uint verticesSize,uint* indices, uint indiceSize,
	const vec3f& min, const vec3f& max, float minSize)
{
	if (vertices != nullptr && indices != nullptr && indiceSize > 0) {

		//Define AABB for Childs
		mRoot = new BSPTreeNode;
		mRoot->Init(min, max, 0, minSize);

		for (int i = 0; i < indiceSize; i += 3){
			mRoot->ReserveIndices(vertices, indices, i, i + 1, i + 2);
		}
		mRoot->ResetAABB(vertices);
		uint nTotalNode = mRoot->counter;

		uint nTotalBufferNode = mRoot->SumitIndexBufferToGPU();

		CreateBuffer(vertices, verticesSize, indices, indiceSize);

		//mQueryNodes = new TreeNode*[nTotalBufferNode];
		mQN.resize(nTotalBufferNode);
	}
}

void BSPTree::CreateBuffer(NodeVertex* vertices, uint verticesSize,uint* indices, uint indicesSize)
{
	//Replace to Terrain Shader and Buffer
	mShader = ShaderCache::CreateShader("basicVS", "basicPS");

	VertexBufferCreateInfo info{};
	info.pVertices = vertices;
	info.VerticesSize = verticesSize * sizeof(NodeVertex);
	info.Type = BufferType::BUFFER_STATIC;
	
	std::vector<VertexAttrib> layout = {
		{ 0, VertexFormat::FLOAT, 3, sizeof(NodeVertex), offsetof(NodeVertex, NodeVertex::pos) },
		{ 0, VertexFormat::FLOAT, 3, sizeof(NodeVertex), offsetof(NodeVertex, NodeVertex::normal) },
	};

	info.AttribSize = layout.size();
	info.pAttrib = layout.data();

	mVertexBuffer = BufferCache::CreateVertexBuffer(&info);
}

uint BSPTree::QueryRenderableNode(Frustum& frustum)
{
	uint nVerticesRendered = 0;
	mVisibleQueryNodesCount = 0;

	if (mRoot) {
		nVerticesRendered = mRoot->QueryVisibility(frustum, 
			mQN.data(), mVisibleQueryNodesCount);
	}
	return nVerticesRendered;
}

void BSPTree::QueryRender()
{
	mShader->Bind();
	mVertexBuffer->Bind();
	if (mVisibleQueryNodesCount > 0)
	{
		for (int i = 0; i < mVisibleQueryNodesCount; ++i)
		{
			mQN[i]->Render();
		}
	}
	mVertexBuffer->UnBind();
}


