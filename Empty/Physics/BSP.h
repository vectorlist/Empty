#pragma once

#include <Physics/Tree/TreeNode.h>

class BSPTreeNode : public TreeNode
{
public:
	BSPTreeNode();
	virtual~BSPTreeNode();
	static uint counter;

	//uint mLevel;
	//AABB mAABB;

	BSPTreeNode* mChild[2];

	//test
	void Init(const vec3f& min, const vec3f& max, uint level, float minaabbSize);

	bool ReserveIndices(NodeVertex* vertice, uint* indices, uint p0, uint p1, uint p2);
	uint SumitIndexBufferToGPU();

	void Render() override;

	void ResetAABB(NodeVertex* vertices);

	uint QueryVisibility(Frustum& frustum,
		TreeNode** queryNodes, uint& visibleCount);

	//uint				ibo;
	bool				IsVisibleLeaf = false;
	float mDistance = 0.0f;
	bool mIsVisible = false;
	
	bool				IsLeaf() const override { return !mChild[0] && !mChild[1]; }	//are u last node no more child?
	std::vector<uint>	mIndices;
	uint				mIndicesCount;
	IndexBuffer*		mIndexBuffer;
};


class BSPTree
{
public:
	BSPTree();

	void Init(NodeVertex* vertices, uint verticesSize,uint* pIndcies, uint indiceSize,
		const vec3f& min, const vec3f& max, float minSize);
	
	BSPTreeNode* mRoot;

	void CreateBuffer(NodeVertex* vertices, uint verticesSize,uint* indices, uint indicesSize);

	uint mIndicesCount;
	Shader* mShader;
	VertexBuffer* mVertexBuffer;

	TreeNode**	  mQueryNodes = nullptr;
	std::vector<TreeNode*> mQN;

	uint mVisibleQueryNodesCount = 0;

	//Get Visible Count and Setting
	uint QueryRenderableNode(Frustum& frustum);
	void QueryRender();
private:
	std::unique_ptr<BSPTreeNode[]> mIndexedNodes;
};

