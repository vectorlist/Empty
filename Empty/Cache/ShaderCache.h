#pragma once

#include <graphics/shader.h>

class ShaderCache
{
public:
	//Internal
	static Shader* CreateSceneShader();
	static Shader* CreateAABBShader();
	static Shader* CreateFontInstanceShader();

	//External
	static Shader* CreateShader(const std::string& vs, const std::string& fs);

	static std::map<std::string, std::shared_ptr<Shader>> mInternalShader;
	static std::map<std::string, std::shared_ptr<Shader>> mExternalShader;
};
