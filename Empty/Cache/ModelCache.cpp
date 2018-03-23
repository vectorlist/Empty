#include <PCH.h>
#include <Cache/ModelCache.h>

#include <Graphics/Context.h>
#include <Graphics/GL4/GLModel.h>
#include <Graphics/DX11/DXModel.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::unordered_map<std::string, std::shared_ptr<Model>> ModelCache::mExternalModels;
std::unordered_map<uint, std::shared_ptr<Model>>		ModelCache::mInternalModels;

void ImportAssimpModel(const std::string& filename, Model* model);

Model* ModelCache::LoadModel(const std::string &filename)
{
	auto found = mExternalModels.find(filename);

	if (found == mExternalModels.end()) {
		std::shared_ptr<Model> model = nullptr;

		switch (GContext->GetApiType())
		{
		case GraphicAPI::OPENGL45:
			model = std::make_shared<GLModel>();
			ImportAssimpModel(filename, model.get());
			break;
		case GraphicAPI::DIRECTX11:
			model = std::make_shared<DXModel>();
			ImportAssimpModel(filename, model.get());
			break;
		default:
			ASSERT_MSG(0, "Invalid API");
			break;
		}

		mExternalModels.insert(std::make_pair(filename, model));
		return model.get();
	}
	return found->second.get();
}

Model* ModelCache::CreateModel(ModelCreateInfo& info)
{
	static uint internalCount = 0;

	std::shared_ptr<Model> model = nullptr;
	switch (GContext->GetApiType())
	{
	case GraphicAPI::OPENGL45:
		model = std::make_shared<GLModel>();
		model->CreateBuffer(info);
		break;
	case GraphicAPI::DIRECTX11:
		model = std::make_shared<DXModel>();
		model->CreateBuffer(info);
		break;
	default:
		ASSERT_MSG(0, "Invalid API");
		break;
	}
	if (model == nullptr) {
		ASSERT_MSG(0,"Faild to Load Model");
	}
	mInternalModels.insert(std::make_pair(internalCount++, model));
	return model.get();
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


