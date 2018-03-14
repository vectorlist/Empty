#pragma once

//TODO Terrain comnpare (BSP, QUADTREE, BVH) trees

struct TerrainCreateInfo
{
	const char* fileName;
	float		cellScale;
	float		heightScale;
	uint		nTiles;
};

struct TerrainVertex
{
	vec3f pos;
	vec3f normal;
	vec2f st;
	vec2f uv;
	//vec3f binormal;
	//vec3f tangent;
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
	
	void CreateTerrain(uint nTile,PixelBuffer* buffer);

	float mHeightScale;
	float mCellScale;

	int mVertexNumX;
	int mVertexNumZ;
	int mVertexNum;

	int mCellNumX;
	int mCellNumZ;
	int mCellNum;

	uint mTotalTriNum;


	//TerrainVertex* mVertices;
	std::vector<TerrainVertex> mVertices;

	float GetHeightFromPixel(int x, int z, PixelBuffer* buffer);

	void SmoothTriangle();

	
	Model* mModel;

	//test;
	Shader* mShader;

	void TestRender();
};

//tmp1 = tmp + nb;
//tmp2 = tmp + 2 * nb;
//
//wt = 1.0 / (nb*nb);
//for (i = 0; i < nx; i++) {
//	x[o + i*d] = (2.*tmp1[i] - tmp[i] - tmp2[i])*wt;
//}