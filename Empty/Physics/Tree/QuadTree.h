#pragma once

#include <Physics/Tree/TreeNode.h>

//#define LT			0
//#define RT			1
//#define LB			2
//#define RB			3
//#define QUAD_MAX	4

//  LT ---TC --- RT
//  |            |
//  LC     C     RC   * CICLE OF RADIUS
//  |            |
//  LB ---BC --- RB

//  Dir  = LT  to C

struct Quad
{
	uint d[4];
	uint& operator[](uint i) {
		return (d)[i];
	}
};

class QuadTreeNode : public TreeNode
{
public:
	enum { LT = 0U, RT, LB, RB, QUAD_MAX };
	QuadTreeNode();
	virtual ~QuadTreeNode();

	void Init(void* someData, uint size, float minSize = 16.f);
	//Four Child

	void BuildQuadTree(float min);
	
	Quad            mQuads;
	QuadTreeNode*	mChild[4];
	NodeVertex*		mVertices = nullptr;
	float			mRadius;
	uint			mCenterIndex = 0;

	virtual bool IsLeaf() const { return mIsLeaf; };
	virtual void Render() override {/*do nothing*/};

	//Test
private:
	bool mIsLeaf;
};

//Point qSize = Point(boundary.halfSize.x, boundary.halfSize.y);
//Point qCentre = Point(boundary.centre.x - qSize.x, boundary.centre.y - qSize.y);
//nw = new Quadtree(AABB(qCentre, qSize));
//
//qCentre = Point(boundary.centre.x + qSize.x, boundary.centre.y - qSize.y);
//ne = new Quadtree(AABB(qCentre, qSize));
//
//qCentre = Point(boundary.centre.x - qSize.x, boundary.centre.y + qSize.y);
//sw = new Quadtree(AABB(qCentre, qSize));
//
//qCentre = Point(boundary.centre.x + qSize.x, boundary.centre.y + qSize.y);
//se = new Quadtree(AABB(qCentre, qSize));
//}