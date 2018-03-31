#pragma once

#include <Graphics/Model.h>

class ModelCache
{
public:

	static Model* LoadModel(const std::string& filename);
	//static Model* CreateModel(ModelCreateInfo& info);
	
	static std::unordered_map<std::string, std::shared_ptr<Model>> mExternalModels;
	//TODO : replace hash key
	static std::unordered_map<uint, std::shared_ptr<Model>> mInternalModels;
};