#include <PCH.h>
#include <World/Terrain.h>

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
	image.LoadFromFile(info.Filename, IMAGE_REQ_RGBA);

	//pixel set by image
	PixelBuffer* buffer = image.GetPixelBuffer();

	//image is not pow2 pixel sets by mipmap level 0
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
	mCellScale = info.CellScale;
	mHeightScale = info.HeightScale;

	
	mShader = ShaderCache::CreateShader("terrainVS", "terrainPS");

	CreateTerrain(info.nTiles, buffer, info.nSmooth);


}

void Terrain::CreateTerrain(uint nTile, PixelBuffer* buffer, uint nSmooth)
{
	mVertices.resize(mVertexNum);

	//UV for cell
	float cellInvX = static_cast<float>(nTile) / mCellNumX;
	float cellInvZ = static_cast<float>(nTile) / mCellNumZ;
	mMin = vec3f( FLT_MAX);
	mMax = vec3f(-FLT_MAX);

	for (int z = 0; z < mVertexNumZ; ++z)
	{
		for (int x = 0; x < mVertexNumX; ++x)
		{
			vec3f pos;

			pos.x = (x - (mCellNumX * 0.5f)) * mCellScale;
			pos.z = (-z + (mCellNumZ * 0.5f)) * mCellScale;
			pos.y = GetHeightFromPixel(x, z, buffer) * mHeightScale;

			mMin = vec3f::min(mMin, pos);
			mMax = vec3f::max(mMax, pos);

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

	this->SmoothTriangle(nSmooth);

	//SRC
	std::vector<vec3f> poses(mVertexNum);
	std::vector<vec2f> sts(mVertexNum);
	
	for (int i =0; i < mVertexNum; ++i)
	{
		poses[i] = mVertices[i].pos;
		sts[i] = mVertices[i].st;
	}

	//DST
	std::vector<vec3f> dstNormals(mVertexNum);
	std::vector<vec3f> dstBinormal(mVertexNum);
	std::vector<vec3f> dstTangent(mVertexNum);

	this->EvalNormal(poses.data(),poses.size(), dstNormals.data(), (uint*)triIndices.data(), triIndices.size() * 3);

	this->EvalBiNormalAndTangent(
		poses.data(), dstNormals.data(), sts.data(),
		mVertexNum, mTotalTriNum, (uint*)triIndices.data(), triIndices.size() * 3,
		dstBinormal.data(), dstTangent.data());

	//MAP TO STRIDE
	index = 0;
	TerrainVertex* P = (TerrainVertex*)mVertices.data();
	while (index < mVertexNum)
	{
		auto srcPtr = P++;
		//increase by byte with stride offset per own byte
		uint8* curPtr = (uint8*)srcPtr + offsetof(TerrainVertex, TerrainVertex::normal);
		memcpy(curPtr, &dstNormals.data()[index], sizeof(float) * 3);
		curPtr = (uint8*)srcPtr + offsetof(TerrainVertex, TerrainVertex::binormal);
		memcpy(curPtr, &dstBinormal.data()[index], sizeof(float) * 3);
		curPtr = (uint8*)srcPtr + offsetof(TerrainVertex, TerrainVertex::tangent);
		memcpy(curPtr, &dstTangent.data()[index], sizeof(float) * 3);
		index++;
	}


	//Model Create
	ModelCreateInfo info{};
	info.pVertices = mVertices.data();
	info.VerticesSize = mVertices.size() * sizeof(TerrainVertex);
	info.pIndices = triIndices.data();
	info.IndicesSize = triIndices.size() * 3 * sizeof(uint);

	std::vector<VertexAttrib> inputs = {
		{ 0, VertexFormat::FLOAT, 3, sizeof(TerrainVertex), offsetof(TerrainVertex, TerrainVertex::pos)},
		{ 0, VertexFormat::FLOAT, 3, sizeof(TerrainVertex), offsetof(TerrainVertex, TerrainVertex::normal) },
		{ 0, VertexFormat::FLOAT, 2, sizeof(TerrainVertex), offsetof(TerrainVertex, TerrainVertex::st) },
		{ 0, VertexFormat::FLOAT, 2, sizeof(TerrainVertex), offsetof(TerrainVertex, TerrainVertex::uv) },
		{ 0, VertexFormat::FLOAT, 3, sizeof(TerrainVertex), offsetof(TerrainVertex, TerrainVertex::binormal) },
		{ 0, VertexFormat::FLOAT, 3, sizeof(TerrainVertex), offsetof(TerrainVertex, TerrainVertex::tangent) }
	};

	info.pAttrib = inputs.data();
	info.AttribSize = inputs.size();

	mModel = ModelCache::CreateModel(info);

	//TODO : BSP or Quadtree

	mNodeVertices.resize(mVertices.size());

	for (int i = 0; i < mVertices.size(); ++i) {
		mNodeVertices[i].pos = mVertices[i].pos;
		mNodeVertices[i].normal = mVertices[i].normal;
	}

	//dont need huge data anymore
	mVertices.clear();

	//BSP
	mTree.Init(mNodeVertices.data(), mNodeVertices.size(),
		(uint*)triIndices.data(), triIndices.size() * 3,
		mMin, mMax, 32.f);

	//QuadTree
	//uint size = mMin.x - mMax.x;		//x z must equal
	uint size = mVertexNumX;

	mQuadTree.Init(mNodeVertices.data(), size);

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
}

//TODO : More Optimize (expensive)
void Terrain::SmoothTriangle(uint level)
{
	if (level <= 0) 
		return;
	std::vector<float> smoothValues(mTotalTriNum);

	while (level > 0)
	{
		--level;

		for (int z = 0; z < mVertexNumZ; ++z)
		{
			for (int x = 0; x < mVertexNumX; ++x)
			{
				int selectCount = 0;
				float total = 0.0f;
				uint smoothIndex = (z * mVertexNumX) + x;

				//gather left
				if (0 < (x - 1))
				{
					total += mVertices[(z * mVertexNumX) + (x - 1)].pos.y;
					selectCount++;
					//gather left top 
					if (0 < (z - 1))
					{
						total += mVertices[((z - 1) * mVertexNumX) + (x - 1)].pos.y;
						selectCount++;
					}
					//gather left bottom
					if (mVertexNumZ > (z + 1))
					{
						total += mVertices[((z + 1) * mVertexNumX) + (x - 1)].pos.y;
						selectCount++;
					}
				}
				//gather right
				if (mVertexNumX > (x + 1))
				{
					total += mVertices[(z * mVertexNumX) + (x + 1)].pos.y;
					selectCount++;
					//gather right top
					if (0 < (z - 1))
					{
						total += mVertices[((z - 1) * mVertexNumX) + (x + 1)].pos.y;
						selectCount++;
					}
					//gather tight bottom
					if (mVertexNumZ > (z + 1))
					{
						total += mVertices[((z + 1) * mVertexNumX) + (x + 1)].pos.y;
						selectCount++;
					}
				}
				//TOP
				if (0 < (z - 1)) {
					total += mVertices[((z - 1) * mVertexNumX) + x].pos.y;
					selectCount++;
				}
				//BOTTOM
				if (mVertexNumZ > (z + 1)) {
					total += mVertices[((z + 1) * mVertexNumX) + x].pos.y;
					selectCount++;
				}

				smoothValues[smoothIndex] = (mVertices[smoothIndex].pos.y + (total / selectCount)) * 0.5f;
				
			}
		}

		LOG << "smoothing remain ++ : " << level + 1 << ENDN;
		for (int i = 0; i < mVertexNum; ++i)
		{
			mVertices[i].pos.y = smoothValues[i];
		}

	} // end of smooth while

}

void Terrain::EvalNormal(vec3f* srcPos, uint verticeSize, vec3f* dstNormal, uint* pIndices, uint indicesSize)
{
	uint triangleCount = indicesSize / 3;

	for (int i = 0; i < triangleCount; ++i)
	{
		uint index0 = pIndices[(i * 3) + 0];
		uint index1 = pIndices[(i * 3) + 1];
		uint index2 = pIndices[(i * 3) + 2];

		vec3f v0 = srcPos[index0];
		vec3f v1 = srcPos[index1];
		vec3f v2 = srcPos[index2];

		vec3f edge0 = v1 - v0;
		vec3f edge1 = v2 - v0;

		vec3f normal = vec3f::cross(edge0, edge1).normalized();

		//gather normals
		dstNormal[index0] += normal;
		dstNormal[index1] += normal;
		dstNormal[index2] += normal;
	}

	//normalize again
	for (uint i = 0; i < verticeSize; ++i) {
		dstNormal[i] = dstNormal[i].normalized();
	}
}


void Terrain::EvalBiNormalAndTangent(
	vec3f* srcPos,
	vec3f* srcNormal,
	vec2f* srcST,
	uint verticeSize,
	uint triangleSize,
	uint* pIndices,
	uint indicesSize,
	vec3f* dstBinormal,
	vec3f* dstTangent)
{
	uint triNum = indicesSize / 3;

	//dummy T, B
	std::vector<vec3f> pBinormal(verticeSize);
	std::vector<vec3f> pTangent(verticeSize);

	for (uint i = 0; i < triNum; ++i)
	{
		uint index0 = pIndices[(i * 3) + 0];
		uint index1 = pIndices[(i * 3) + 1];
		uint index2 = pIndices[(i * 3) + 2];

		vec3f v0 = srcPos[index0];
		vec3f v1 = srcPos[index1];
		vec3f v2 = srcPos[index2];

		vec3f edge0 = v1 - v0;
		vec3f edge1 = v2 - v0;

		//get ST of Triangle
		vec2f st0 = srcST[index0];
		vec2f st1 = srcST[index2];
		vec2f st2 = srcST[index1];

		//st edge
		vec2f stEdge0 = st1 - st0;
		vec2f stEdge1 = st2 - st0;

		//Equation
		//EDGE0 = (STEDGE1.x) * TANGENT + (STEDGE1.y) * BINORMAL
		//EDGE1 = (STEDGE2.x) * TANGENT + (STEDGE1.y) * BINORMAL

		// E0 = EDGE0
		// E1 = EDGE1
		// S0 = STEdge0.x
		// T0 = STEdge0.y
		// S1 = STEdge1.x
		// T1 = STEdge1.y
		// T = Tangent
		// B = Binrmal

		// E0 = S0 * T * T0 * B
		// E1 = S1 * T * T1 * B

		// | E0 |   | S0 , T0 | | T |
		// |    | = |		  | |   |
		// | E1 |   | S1 , T1 | | B |

		// | T |		  1				| T1 ,  -T0 | | E0 |
		// |   | = -------------------	|           | |    |
		// | B |	S0 * T1 - T0 * S1	|-S1 ,   S0 | | E1 |

		// D = 1 / S0 * T1 - T0 * S1

		// T  = (E0 * T1) + (E1 * -T0) * D 
		// B  = (E0 *-S1) + (E1 * -S0) * D

		float D = 1.0f / ((stEdge0.x * stEdge1.y) - (stEdge0.y * stEdge1.x));

		vec3f T = (((edge0 * stEdge1.y) + (edge1 * -stEdge0.y)) * D).normalized();
		vec3f B = (((edge0 * -stEdge1.x) + (edge1 * stEdge0.x)) * D).normalized();

		pBinormal[index0] += B;
		pBinormal[index1] += B;
		pBinormal[index2] += B;
		pTangent[index0] += T;
		pTangent[index0] += T;
		pTangent[index0] += T;

	}

	//cross
	for (int i = 0; i < verticeSize; ++i)
	{
		vec3f N = srcNormal[i];
		
		//T = T - (N¡¤T) N
		//U = U - (N¡¤U) N - (T¡¤U) T

		vec3f T = (pTangent[i] - (vec3f::dot(N, pTangent[i])) * N).normalized();

		vec3f B = vec3f::cross(N, T).normalized();

		dstBinormal[i] = B;
		dstTangent[i] = T;
	}
}


void Terrain::Render(Frustum* frustum)
{
	//two waty to render
	if (frustum == nullptr) {
		//Brute Force Render
		mShader->Bind();
		mModel->Render();
		return;
	}
	result.renderedVertices = mTree.QueryRenderableNode(*frustum);
	//Query Visible Node Render
	mTree.QueryRender();
}



