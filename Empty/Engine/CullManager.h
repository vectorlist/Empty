#pragma once


#include <Physics/BVH.h>
//TODO : TYpe of Culling
// OBJECT / BOUND BOX / SPHERE /TERRAIN SEARCH NODE ..ETC

class CullManager
{
public:
	CullManager();
	~CullManager();

	//TODO collective Objects


	//void AddElemnet(Object* object);

	void BuildSeachNode(std::vector<Object*>* objects);

	//Expensive
	//std::vector<Object*> mObjects;

	BVHTreeNode* mRootNode;

	//TODO SEARCH FRUSTUM FROM ROOT (QUERY)

	void DoCulling(Frustum& frustum, std::vector<Object*>& output);


};