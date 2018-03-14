#pragma once


//Init with AABB

class BSPTreeNode
{
public:
	BSPTreeNode();
	static uint counter;

	vec3f min;
	vec3f max;
	uint mLevel;
	AABB mAABB;

	BSPTreeNode* mChild[2];

	//test
	void Init(const vec3f& min, const vec3f& max, uint level, float minaabbSize);


	//available triagnles indices num
	uint	mIndicesCount;

	//indices ptr
	uint*	mIndices;

	bool InsertTraingles(NodeVertex* vertice, uint* indices, uint p0, uint p1, uint p2);
};


class BSPTree
{
public:
	BSPTree();

	void Init(NodeVertex* vertices, uint* pIndcies, uint indiceSize,
		const vec3f& min, const vec3f& max, float minSize);
	
	BSPTreeNode* mRoot;
};

//for (i = 0; i < 2; i++) {
//
//	node->child[i] = (BinNodePtr)malloc(sizeof(BinNode));
//	node->child[i]->min.x = node->min.x;
//	node->child[i]->min.y = node->min.y;
//	node->child[i]->min.z = node->min.z;
//	node->child[i]->max.x = node->max.x;
//	node->child[i]->max.y = node->max.y;
//	node->child[i]->max.z = node->max.z;
//
//	if (axis == 1) {
//
//		/* current subdivision plane is x */
//		node->child[i]->min.x =
//			node->min.x + 0.5 * i * (node->max.x - node->min.x);
//		node->child[i]->max.x =
//			node->min.x + 0.5 * (i + 1) * (node->max.x - node->min.x);
//
//		/* child subdivision plane will be y */
//		nextAxis = 2;
//		node->child[i]->DistanceToDivisionPlane = DistanceToYPlane;
//		node->child[i]->GetChildren = GetYChildren;
//
//	}
//	else if (axis == 2) {
//
//		/* current subdivision plane is y */
//
//		node->child[i]->min.y =
//			node->min.y + 0.5 * i * (node->max.y - node->min.y);
//		node->child[i]->max.y =
//			node->min.y + 0.5 * (i + 1) * (node->max.y - node->min.y);
//
//		/* child subdivision plane will be z */
//		nextAxis = 3;
//		node->child[i]->DistanceToDivisionPlane = DistanceToZPlane;
//		node->child[i]->GetChildren = GetZChildren;
//
//	}
//	else {
//
//		/* current subdivision plane is z */
//		node->child[i]->min.z =
//			node->min.z + 0.5 * i * (node->max.z - node->min.z);
//		node->child[i]->max.z =
//			node->min.z + 0.5 * (i + 1) * (node->max.z - node->min.z);
//
//		/* child subdivision plane will be x */
//		nextAxis = 1;
//		node->child[i]->DistanceToDivisionPlane = DistanceToXPlane;
//		node->child[i]->GetChildren = GetXChildren;
//	}
//
//	ObjPtr = FirstOfLinkList(node->members);
//	while (ObjPtr != NULL) {
//		if (GeomInNode(node->child[i], ObjPtr))
//			AddToLinkList(node->child[i]->members, ObjPtr);
//		ObjPtr = NextOfLinkList(node->members);
//	}
//	Subdivide(node->child[i], depth + 1, MaxDepth, MaxListLength, nextAxis);