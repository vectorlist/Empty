#pragma once

//TODO Terrain comnpare (BSP, QUADTREE, BVH) trees

struct TerrainCreateInfo
{
	const char* fileName;
	float		cellScale;
	float		heightScale;
	uint		nTiles;
	uint		nSmooth;
};

struct TerrainVertex
{
	vec3f pos;
	vec3f normal;
	vec2f st;
	vec2f uv;
	vec3f binormal;
	vec3f tangent;
};

struct TriangleIndex
{
	uint p0;
	uint p1;
	uint p2;
};

struct PixelBuffer;
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

	//test;
	Shader* mShader;

	void TestRender();

	
	std::vector<TerrainVertex>		mVertices;

	//TREE
	vec3f mMin;
	vec3f mMax;
	std::vector<TerrainNodeVertex>	mNodeVertices;
};

