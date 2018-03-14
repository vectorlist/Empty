#include <PCH.h>
#include <Physics/BSP.h>

uint BSPTreeNode::counter = 0;

BSPTreeNode::BSPTreeNode()
	: min(FLT_MAX), max(-FLT_MAX), mIndicesCount(0), mIndices(nullptr)
{
	mChild[0] = nullptr;
	mChild[1] = nullptr;
}

void BSPTreeNode::Init(const vec3f& min, const vec3f& max, uint level, float minaabbSize)
{
	this->min = min;
	this->max = max;

	this->mLevel = level;

	vec3f center = (min + max) * 0.5f;
	vec3f size = max - min;

	mAABB.SetVertices(min, max);

	if (size.x > minaabbSize || size.z > minaabbSize)
	{
		mChild[0] = new BSPTreeNode;
		mChild[1] = new BSPTreeNode;

		//priority x 
		if (size.x >= size.z) {
			mChild[0]->Init(vec3f(min.x, min.y, min.z), vec3f(center.x, max.y, max.z), level + 1, minaabbSize);
			mChild[1]->Init(vec3f(center.x, min.y, min.z), vec3f(max.x, max.y, max.z), level + 1, minaabbSize);
		}
		else {
			mChild[0]->Init(vec3f(min.x, min.y, min.z), vec3f(max.x, max.y, center.z), level + 1, minaabbSize);
			mChild[0]->Init(vec3f(min.x, min.y, center.z), vec3f(max.x, max.y, max.z), level + 1, minaabbSize);
		}
	}

}

bool BSPTreeNode::InsertTraingles(NodeVertex* vertice, uint* indices, uint p0, uint p1, uint p2)
{
	return false;
}

BSPTree::BSPTree()
{
}

void BSPTree::Init(NodeVertex* vertices, uint* pIndcies, uint indiceSize,
	const vec3f& min, const vec3f& max, float minSize)
{
	if (vertices == nullptr && pIndcies == nullptr && indiceSize <= 0) {
		return;
	}

	mRoot = new BSPTreeNode;
	mRoot->Init(min, max, 0, minSize);

	//TODO : Add Triangles Indices

	for (int i = 0; i < indiceSize; ++i)
	{
		//check inside with point AABB
	}

}

