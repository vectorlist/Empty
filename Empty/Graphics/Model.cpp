#include <PCH.h>
#include <Graphics/Model.h>
#include <Graphics/Context.h>
#include <core/log.h>

#include <Graphics/GL4/GLModel.h>
#include <Graphics/DX11/DXModel.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model* Model::CreateModel(std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
	Model* model = nullptr;
	switch (G_Context->GetApiType())
	{
	case GraphicAPI::OPENGL45:
	{
		break;
	}
	default:
		break;
	}
	return model;
}

void ImportAssimpModel(const std::string& filename, Model* model);

//TODO Resource Cache
Model* Model::CreateModelFromFile(const std::string & filename)
{
	Model* model = nullptr;
	switch (G_Context->GetApiType())
	{
	case GraphicAPI::OPENGL45:
		model = new GLModel;
		ImportAssimpModel(filename, model);
		break;
	case GraphicAPI::DIRECTX11:
		model = new DXModel;
		ImportAssimpModel(filename, model);
		break;
	}
	return model;
}

void ImportAssimpModel(const std::string& filename, Model* model)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate |
		aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene) LOG_ERROR("failed to load scene");

	std::vector<Vertex> vertices;
	std::vector<uint> indices;

	vec3f min(FLT_MAX);
	vec3f max(-FLT_MAX);

	for (int n = 0; n < scene->mNumMeshes; ++n)
	{
		aiMesh* mesh = scene->mMeshes[n];

		for (int i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;
			memcpy(&vertex.pos, &mesh->mVertices[i], (size_t)(sizeof(float) * 3));
			memcpy(&vertex.normal, &mesh->mNormals[i], (size_t)(sizeof(float) * 3));

			min = vec3f::min(min, vertex.pos);
			max = vec3f::max(max, vertex.pos);

			if (mesh->mTextureCoords[0])
				memcpy(&vertex.st, &mesh->mTextureCoords[0][i], sizeof(vec2f));
			else
				vertex.st = vec2f(0.0f);

			vertex.binormal.x = mesh->mBitangents[i].x;
			vertex.binormal.y = mesh->mBitangents[i].y;
			vertex.binormal.z = mesh->mBitangents[i].z;

			vertex.tangent.x = mesh->mTangents[i].x;
			vertex.tangent.y = mesh->mTangents[i].y;
			vertex.tangent.z = mesh->mTangents[i].z;

			vertices.emplace_back(vertex);
		}

		for (int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (int f = 0; f < face.mNumIndices; ++f)
			{
				indices.emplace_back(face.mIndices[f]);
			}
		}
	}

	model->mAABB = AABB(min, max);
	model->CreateBuffer(vertices, indices);
}