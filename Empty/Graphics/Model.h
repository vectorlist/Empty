#pragma once

class Mesh
{
public:
	std::vector<vec3f> vertices;
	void SetIndexedVertices(Vertex* pVtx, uint* pIdx, uint idxSize)
	{
		for (uint i = 0; i < idxSize; ++i)
		{
			uint index = pIdx[i];
			vec3f pos = pVtx[index].pos;
			vertices.emplace_back(pVtx[index].pos);
		}
	}
};


class Model
{
public:
	Model() : mIndiceNum(0) , mHasBuffer(false){}
	virtual~Model(){}

	virtual void CreateBuffer(std::vector<Vertex> &vertice, std::vector<uint>& indices) = 0;
	virtual void CreateBuffer(Vertex* pVertices, uint vertexSize, uint* pIndices, uint indicesSize) = 0;
	virtual void Bind() {};
	virtual void Render() {};

	bool HasBuffer() const { return mHasBuffer; }
	uint GetIndicesNum() const { return mIndiceNum; }
	
	static Model* CreateModel(std::vector<Vertex>& vertices, std::vector<uint>& indices);
	static Model* CreateModelFromFile(const std::string& filename);

	AABB mAABB;
protected:
	uint mIndiceNum;
	bool mHasBuffer;
	Mesh mMesh;
};