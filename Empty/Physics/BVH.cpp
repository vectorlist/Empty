#include <PCH.h>
#include "Physics/BVH.h"
#include <Core/object.h>
#include <Physics/Physics.h>

BVHTreeNode::BVHTreeNode()
	: mLeft(nullptr), mRight(nullptr), mLevel(0)
{
}

BVHTreeNode::BVHTreeNode(AABB aabb, std::vector<Object*>& objects, BVHTreeNode* left, BVHTreeNode* right, uint level)
	: mAabb(aabb), mObjects(objects), mLeft(left), mRight(right), mLevel(level)
{

}

BVHTreeNode* BVHTreeNode::Build(std::vector<Object*>& objects, uint level)
{
	BVHTreeNode* node = nullptr;
	BVHTreeNode* leftNode = nullptr;
	BVHTreeNode* rightNode = nullptr;
	//TODO : add parent here
	//Get Total AABB
	AABB aabb = Physics::GetTotalAABBFromObjects(objects.data(), objects.size());

	//WHILE OBJECTS REMAINS AT LEAST 2 (SPLIT ABLE)
	if (objects.size() > 1) {
		std::vector<Object*> leftObjs;
		std::vector<Object*> rightObjs;
		//SPLIT BY AXIS AND GET LONGEST VALUE
		AABB::Axis axis = aabb.GetLongestAxis();
		//GET MID POINT (WOLRD)
		vec3f midPoint = aabb.GetMidPoint();
		//GET LONGEST VALUE
		float longestVal = midPoint[axis];

		for (uint i = 0; i < objects.size(); ++i) {
			AABB subAABB = objects[i]->GetWorldAABB();
			//Get axis value from per object by midpoint
			float subLongestVal = subAABB.GetMidPoint()[axis];
			if (longestVal < subLongestVal) {
				leftObjs.emplace_back(objects[i]);
			}
			else {
				rightObjs.emplace_back(objects[i]);
			}
		}
		//RECURSIVE
		leftNode = Build(leftObjs, level + 1);
		rightNode = Build(rightObjs, level + 1);
	}

	//WHILE OBJECTS REMAINS 
	if (objects.size() > 0) {
		node = new BVHTreeNode(aabb, objects, leftNode, rightNode, level);
	}
	//RETURN NULL WHEN OBJECT SIZE 0
	return node;
}



void BVHTreeNode::RenderAllNode()
{
	////test is node leaf?
	//if (this->IsLeaf()) {
	//	G_DebugBatch->RenderAABB(&mAabb, Matrix4x4(), vec3f(1, 1, 1));
	//}
	//if (mLeft) {
	//	RenderAllNode();
	//}
	//if (mRight) {
	//	RenderAllNode();
	//}

}
