#pragma once

//TODO Inherite Renderable Tree Node

class BVHTreeNode
{
public:
	BVHTreeNode();
	BVHTreeNode(AABB aabb, std::vector<Object*>& objects, BVHTreeNode* left, BVHTreeNode* right, uint level);

	BVHTreeNode* mLeft;
	BVHTreeNode* mRight;

	//Current node depth level (Root == 0, Leaf = last of N)
	uint mLevel;		

	//total world AABB
	AABB mAabb;
	AABB* GetAABB() { return  &mAabb; }
	//Expensive BVH
	std::vector<Object*> mObjects;

	static BVHTreeNode* Build(std::vector<Object*>& objects, uint level);

	bool DoCulling(Frustum& frustum);

	bool IsLeaf() const { return mLeft == nullptr && mRight == nullptr; }

	//(Hold any of type) (instance object, matrix, properties)
	void* customPtr = nullptr;
};