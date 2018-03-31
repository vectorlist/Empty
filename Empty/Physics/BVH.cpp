#include <Core/PCH.h>
#include "Physics/BVH.h"
#include <Core/object.h>
#include <Physics/Physics.h>

BVHTreeNode::BVHTreeNode()
	: mLeft(nullptr), mRight(nullptr), mLevel(0)
{
}

BVHTreeNode::BVHTreeNode(AABB aabb, std::vector<Object*>& objects, BVHTreeNode* left, BVHTreeNode* right, uint level)
	: mAabb(aabb),mObjects(objects), mLeft(left), mRight(right), mLevel(level)
{
	//TODO : More generic (not on contructor)
	//Check if leaf and Object is 1
	if (this->IsLeaf() && objects.size() == 1) {
		this->customPtr = &objects[0]->GetGlobal();
	}
}

//BVHTreeNode* BVHTreeNode::Build(std::vector<Object*>& objects, uint level)
//{
//	BVHTreeNode* node = nullptr;
//	BVHTreeNode* leftNode = nullptr;
//	BVHTreeNode* rightNode = nullptr;
//	//TODO : add parent here
//	//Get Total AABB
//	AABB aabb = Physics::GetTotalAABBFromObjects(objects.data(), objects.size());
//
//	//WHILE OBJECTS REMAINS AT LEAST 2 (SPLIT ABLE)
//	if (objects.size() > 1) {
//		std::vector<Object*> leftObjs;
//		std::vector<Object*> rightObjs;
//		//SPLIT BY AXIS AND GET LONGEST VALUE
//		AABB::Axis axis = aabb.GetLongestAxis();
//		//GET MID POINT (WOLRD)
//		vec3f midPoint = aabb.GetMidPoint();
//		//GET LONGEST VALUE
//		float longestVal = midPoint[axis];
//
//		for (uint i = 0; i < objects.size(); ++i) {
//			AABB subAABB = objects[i]->GetWorldAABB();
//			//Get axis value from per object by midpoint
//			vec3f subMid = subAABB.GetMidPoint();
//			float subLongestVal = subMid[axis];
//			if (longestVal < subLongestVal) {
//				leftObjs.emplace_back(objects[i]);
//			}
//			else {
//				rightObjs.emplace_back(objects[i]);
//			}
//		}
//		//RECURSIVE
//		leftNode = Build(leftObjs, level + 1);
//		rightNode = Build(rightObjs, level + 1);
//	}
//
//	//WHILE OBJECTS REMAINS 
//	if (objects.size() > 0) {
//		node = new BVHTreeNode(aabb, objects, leftNode, rightNode, level);
//	}
//	//RETURN NULL WHEN OBJECT SIZE 0
//	return node;
//}

bool SortX(Object* a, Object* b);
bool SortY(Object* a, Object* b);
bool SortZ(Object* a, Object* b);

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
		
		switch (axis)
		{
		case AABB::Axis::X:
			std::sort(objects.begin(), objects.end(), SortX);
			break;
		case AABB::Axis::Y:
			std::sort(objects.begin(), objects.end(), SortY);
			break;
		case AABB::Axis::Z:
			std::sort(objects.begin(), objects.end(), SortZ);
			break;
		}
		const int midIndex = objects.size() / 2;

		leftObjs.assign(objects.begin(), objects.begin() + midIndex);
		rightObjs.assign(objects.begin() + midIndex, objects.end());

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

bool SortX(Object* a, Object* b) { 
	return b->GetWorldAABB().GetMidPoint().x > b->GetWorldAABB().GetMidPoint().x; 
}

bool SortY(Object* a, Object* b) {
	return b->GetWorldAABB().GetMidPoint().y > b->GetWorldAABB().GetMidPoint().y;
}
bool SortZ(Object* a, Object* b) {
	return b->GetWorldAABB().GetMidPoint().z > b->GetWorldAABB().GetMidPoint().z;
}
bool BVHTreeNode::DoCulling(Frustum& frustum)
{
	if (frustum.IsInAABBSphere(mAabb.GetCenter(), mAabb.GetRadius())) {
		return true;
	}
	return false;
}




