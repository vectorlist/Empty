#pragma once

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
	//TODO : template <Object T>
	std::vector<Object*> mObjects;
	static BVHTreeNode* Build(std::vector<Object*>& objects, uint level);

	void RenderAllNode();

	bool IsLeaf() const { return mLeft == nullptr && mRight == nullptr; }
};