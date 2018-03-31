#include <Core/PCH.h>
#include <Engine/CullManager.h>

//test

CullManager::CullManager()
	: mRootNode(nullptr)
{
}

CullManager::~CullManager()
{
	SAFE_DELETE(mRootNode);
}

//void CullManager::AddElemnet(Object* object)
//{
//	mObjects.emplace_back(object);
//}

void CullManager::BuildSeachNode(std::vector<Object*>* objects)
{
	if (objects->size() == 0) return;
	mRootNode = BVHTreeNode::Build(*objects, 0);

	//Get total Node
}

void CullManager::DoCulling(Frustum& frustum, std::vector<Object*>& output)
{
	//TODO 1 : fighting SPHERES
	//TODO 2 : check frustum
	//TODO 3 : check leaf node and add object
	
	auto node = mRootNode;

	if (node->DoCulling(frustum)) {

		if (node->mLeft) {
			node->mLeft->DoCulling(frustum);
		}
		if (node->mRight) {
			node->mRight->DoCulling(frustum);
		}
	}
}
