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

//Spartaner_v1: std::string ext = (type == "glsl") ? ("glsl_vs", "hlsl_vs");
//Spartaner_v1: std::string fullUrL = url + std::string(".") + ext;
//Spartaner_v1: if (fileExists(fullUrl)) { /* File exists */ }
//			  else { /* File not found */ }

//fs::path p = fs::absolute(path);
//fs::path r = fs::absolute(relative_to);
//
//// if root paths are different, return absolute path
//if (p.root_path() != r.root_path())
//return p;
//
//// initialize relative path
//fs::path result;