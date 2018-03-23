#pragma once

//TODO Terrain comnpare (BSP, QUADTREE, BVH) trees

#include <Physics/BSP.h>
#include <Physics/Tree/QuadTree.h>

class Terrain
{
public:
	//TODO : intt from file/ init from binary / init form noise / init from model
	Terrain();

	void Init(TerrainCreateInfo& info);
	
	void CreateTerrain(uint nTile,PixelBuffer* buffer, uint nSmooth);

	float mHeightScale;
	float mCellScale;

	int mVertexNumX;
	int mVertexNumZ;
	int mVertexNum;

	int mCellNumX;
	int mCellNumZ;
	int mCellNum;

	uint mTotalTriNum;

	float GetHeightFromPixel(int x, int z, PixelBuffer* buffer);

	void SmoothTriangle(uint level);
	void EvalNormal(vec3f* srcPos, uint verticeSize, vec3f* dstNormal, uint* pIndices, uint indicesSize);

	void EvalBiNormalAndTangent(
		vec3f* srcPos,
		vec3f* srcNormal,
		vec2f* srcST,
		uint verticeSize,
		uint triangleSize,
		uint* pIndices,
		uint indicesSize,
		vec3f* dstBinormal,
		vec3f* dstTangent
	);
	
	Model* mModel;
	Shader* mShader;	
	std::vector<TerrainVertex>		mVertices;

	vec3f mMin;
	vec3f mMax;
	std::vector<TerrainNodeVertex>	mNodeVertices;

	//Test Trees
	BSPTree mTree;
	QuadTreeNode mQuadTree;

	Camera* mCamera;

	void Render(Frustum* frustum = nullptr);

	struct 
	{
		uint renderedVertices = 0;
		uint renderedNodes = 0;
	}result;
};

