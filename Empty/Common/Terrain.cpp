#include <PCH.h>
#include <Common/Terrain.h>

#include <Image/Image.h>
#include <Cache/BufferCache.h>
#include <Cache/ModelCache.h>

#include <Cache/ShaderCache.h>

Terrain::Terrain()
{
	
}

void Terrain::Init(TerrainCreateInfo& info)
{
	Image image;
	image.LoadFromFile(info.fileName, IMAGE_REQ_RGBA);

	//check pow2 and create mipmap
	PixelBuffer* buffer = image.GetPixelBuffer();

	if(!image.IsPow2()){
		LOG << "this image is not pow2 Generate Mipmap..." << ENDN;
		Mipmap* map = image.CreateMipmaps();
		buffer = map->GetPixelBuffer(0);

	}

	mVertexNumX = buffer->width + 1;
	mVertexNumZ = buffer->height + 1;
	mVertexNum = mVertexNumX * mVertexNumZ;

	mCellNumX = mVertexNumX - 1;
	mCellNumZ = mVertexNumZ - 1;
	mCellNum = mCellNumX * mCellNumZ;

	mTotalTriNum = mCellNum * 2;
	mCellScale = info.cellScale;
	mHeightScale = info.heightScale;

	//test without ext
	mShader = ShaderCache::CreateShader("../data/shader/terrainVS", "../data/shader/terrainPS");

	CreateTerrain(info.nTiles, buffer);


}

void Terrain::CreateTerrain(uint nTile, PixelBuffer* buffer)
{
	mVertices.resize(mVertexNum);

	//UV for cell
	float cellInvX = static_cast<float>(nTile) / mCellNumX;
	float cellInvZ = static_cast<float>(nTile) / mCellNumZ;

	for (int z = 0; z < mVertexNumZ; ++z)
	{
		for (int x = 0; x < mVertexNumX; ++x)
		{
			vec3f pos;

			pos.x = (x - (mCellNumX * 0.5f)) * mCellScale;
			pos.z = (-z + (mCellNumZ * 0.5f)) * mCellScale;
			pos.y = GetHeightFromPixel(x, z, buffer) * mHeightScale;

			vec2f st;
			st.x = x / (float)(mVertexNumX - 1);
			st.y = z / (float)(mVertexNumZ - 1);

			vec2f uv;
			uv.x = x * cellInvX;
			uv.y = z * cellInvZ;

			//index
			uint index = z * mVertexNumX + x;

			mVertices[index].pos = pos;
			mVertices[index].st = st;
			mVertices[index].uv = uv;
		}
	}

	//Get Indices
	std::vector<TriangleIndex> triIndices(mTotalTriNum);

	int index = 0;

	for (uint z = 0; z < mCellNumZ; ++z)
	{
		for (uint x = 0; x < mCellNumX; ++x)
		{
			// LT -- RT
			// |    /|
			// |   / |
			// |  /  |
			// | /   |
			// |/    |
			// LB -- RB

			uint LT = z * mVertexNumX + x;
			uint RT = z * mVertexNumX + x + 1;
			uint LB = ((z + 1) * mVertexNumX) + x;
			uint RB = ((z + 1) * mVertexNumX) + x + 1;

			//First Triangle
			triIndices[index].p0 = LT;
			triIndices[index].p1 = RT;
			triIndices[index].p2 = LB;
			index++;

			triIndices[index].p0 = LB;
			triIndices[index].p1 = RT;
			triIndices[index].p2 = RB;
			index++;
		}
	}

	//TODO : VertexBuffer and Index Buffer without model

	ModelCreateInfo info{};
	info.pVertices = mVertices.data();
	info.verticesSize = mVertices.size() * sizeof(TerrainVertex);
	info.pIndices = triIndices.data();
	info.indicesSize = triIndices.size() * 3 * sizeof(uint);

	std::vector<VertexInputAttib> inputs = {
		{ 0, VertexFormat::FLOAT, 3, sizeof(TerrainVertex), offsetof(TerrainVertex, TerrainVertex::pos)},
		{ 0, VertexFormat::FLOAT, 3, sizeof(TerrainVertex), offsetof(TerrainVertex, TerrainVertex::normal) },
		{ 0, VertexFormat::FLOAT, 2, sizeof(TerrainVertex), offsetof(TerrainVertex, TerrainVertex::st) },
		{ 0, VertexFormat::FLOAT, 2, sizeof(TerrainVertex), offsetof(TerrainVertex, TerrainVertex::uv) }
	};

	info.pInputAttrib = inputs.data();
	info.inputAttibSize = inputs.size();

	mModel = ModelCache::CreateModel(info);
	


	
}

float Terrain::GetHeightFromPixel(int x, int z, PixelBuffer* buffer)
{
	uint8* P = buffer->pData + (z * buffer->width  + x) * buffer->bytePerPixel;

	float inv = 1.0f / 255.0f;
	float r = *P++ * inv;
	float g = *P++ * inv;
	float b = *P++ * inv;

	float rgb = (r + g + b) / 3.0f;

	return rgb;

	//RGBA8* P = (RGBA8*)buffer->pData + z * buffer->width + x;

	////test
	//float inv = 1.0f / 255.0f;
	//float r = P->r * inv;
	//float g = P->g * inv;
	//float b = P->b * inv;

	//float rgb = (r + g + b) / 3.0f;

	//return rgb;
}

void Terrain::SmoothTriangle()
{
	//TODO : 
}

void Terrain::TestRender()
{
	mShader->Bind();

	mModel->Render();
}



