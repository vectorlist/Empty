#include <PCH.h>
#include <Physics/Tree/QuadTree.h>

QuadTreeNode::QuadTreeNode()
{
	memset(mChild, 0, sizeof(mChild));
}

QuadTreeNode::~QuadTreeNode()
{
	for (uint i = 0; i < QUAD_MAX; ++i) {
		SAFE_DELETE(mChild[i]);
	}
}

void QuadTreeNode::Init(void* someData, uint size, float minSize)
{
	mVertices = static_cast<NodeVertex*>(someData);

	
	mQuads[LT] = 0;
	mQuads[RT] = size - 1;
	mQuads[LB] = (size - 1) * size;
	mQuads[RB] = size * size - 1;

	vec3f minpos = mVertices[mQuads[LT]].pos;
	vec3f maxpos = mVertices[mQuads[RB]].pos;

	mAABB.Init(minpos, maxpos);
	BuildQuadTree(minSize);
}

//TODO : Optimize in simple
void QuadTreeNode::BuildQuadTree(float min)
{
	//TODO : Get Center of Quad
	vec3f center;
	for (int i = 0; i < 4; ++i) {
		center += mVertices[mQuads[i]].pos;
	}
	center /= 4.f; //must be almost 0

	//Get length of any corner
	vec3f dir = mVertices[mQuads[LT]].pos - center;

	//get radius
	float radiusSQ = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;

	mRadius = std::sqrtf(radiusSQ);

	//Condition while quad bigger than min
	if ((mQuads[RT] - mQuads[LT]) > min)
	{
		//Get Center Index
		uint C = 0;
		for (int i = 0; i < 4; ++i) {
			C += mQuads[i];
		}
		C /= 4;
		 
		//re calc quad

		uint TC = mQuads[RT] + mQuads[LT] / 2;
		uint LC = mQuads[LT] + mQuads[LB] / 2;
		uint RC = mQuads[RT] + mQuads[RB] / 2;
		uint BC = mQuads[RB] + mQuads[LB] / 2;

		//create child
		//LT 
		mChild[LT] = new QuadTreeNode;
		mChild[LT]->mQuads[LT] = mQuads[LT];
		mChild[LT]->mQuads[RT] = TC;
		mChild[LT]->mQuads[LB] = LC;
		mChild[LT]->mQuads[LT] = C;
		
		vec3f minpos = mVertices[mChild[LT]->mQuads[LT]].pos;
		vec3f maxpos = mVertices[mChild[LT]->mQuads[RB]].pos;
		mChild[LT]->mAABB.Init(minpos, maxpos);
		

		//RT
		mChild[RT] = new QuadTreeNode;
		mChild[RT]->mQuads[LT] = TC;
		mChild[RT]->mQuads[RT] = mQuads[RT];
		mChild[RT]->mQuads[LB] = C;
		mChild[RT]->mQuads[LT] = RC;

		//LB
		mChild[LB] = new QuadTreeNode;
		mChild[LB]->mQuads[LT] = LC;
		mChild[LB]->mQuads[RT] = C;
		mChild[LB]->mQuads[LB] = mQuads[LB];
		mChild[LB]->mQuads[LT] = BC;

		//RB
		mChild[LB] = new QuadTreeNode;
		mChild[LB]->mQuads[LT] = C;
		mChild[LB]->mQuads[RT] = RC;
		mChild[LB]->mQuads[LB] = BC;
		mChild[LB]->mQuads[LT] = mQuads[RB];

		

	}

	int g = 10;

}
