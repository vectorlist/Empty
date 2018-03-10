#include <PCH.h>
#include <Cache/ShaderCache.h>
#include <Graphics/BuiltInShader.h>
#include <Graphics/Context.h>

#include <Core/FileSystem.h>

std::map<std::string, std::shared_ptr<Shader>> ShaderCache::mInternalShader;
std::map<std::string, std::shared_ptr<Shader>> ShaderCache::mExternalShader;

Shader* ShaderCache::CreateSceneShader()
{
	auto found = mInternalShader.find("scene");
	if (found != mInternalShader.end()) {
		return found->second.get();
	}

	std::shared_ptr<Shader> shader = nullptr;

	switch (G_Context->GetApiType())
	{
	case GraphicAPI::OPENGL45:
		shader = std::make_shared<GLInternalShader>();
		shader->InitFromCode(glsl::sceneVS, glsl::sceneFS);
		break;
	case GraphicAPI::DIRECTX11:
		shader = std::make_shared<DXInternalSceneShader>();
		shader->InitFromCode(hlsl::sceneVS, hlsl::sceneFS);
		break;
	}

	mInternalShader.insert(std::make_pair("scene", shader));

	return shader.get();
}

Shader* ShaderCache::CreateAABBShader()
{
	auto found = mInternalShader.find("aabb");
	if (found != mInternalShader.end()) {
		return found->second.get();
	}

	std::shared_ptr<Shader> shader = nullptr;

	switch (G_Context->GetApiType())
	{
	case GraphicAPI::OPENGL45:
		shader = std::make_shared<GLInternalShader>();
		shader->InitFromCode(glsl::aabbVS, glsl::aabbFS);
		break;
	case GraphicAPI::DIRECTX11:
		shader = std::make_shared<DXInternalAABBShader>();
		shader->InitFromCode(hlsl::aabbVS, hlsl::aabbFS);
		break;
	}

	mInternalShader.insert(std::make_pair("aabb", shader));

	return shader.get();
}

Shader* ShaderCache::CreateFontInstanceShader()
{
	auto found = mInternalShader.find("fontinstance");
	if (found != mInternalShader.end()) {
		return found->second.get();
	}

	std::shared_ptr<Shader> shader = nullptr;

	switch (G_Context->GetApiType())
	{
	case GraphicAPI::OPENGL45:
		shader = std::make_shared<GLInternalShader>();
		shader->InitFromCode(glsl::instanceFontVS, glsl::instanceFontFS);
		break;
	case GraphicAPI::DIRECTX11:
		shader = std::make_shared<DXInternalFontShader>();
		shader->InitFromCode(hlsl::instanceFontVS, hlsl::instanceFontFS);
		break;
	}

	mInternalShader.insert(std::make_pair("fontinstance", shader));

	return shader.get();
}

Shader* ShaderCache::CreateShader(const std::string& vs, const std::string& fs)
{
	//TODO : Check available EXT

	//TODO : check hlsl glsl for extension
	auto vsExt = FileSystem::GetEXTFromPath(vs);

	std::string vert;
	std::string frag;
	if (G_Context->GetApiType() == GraphicAPI::OPENGL45) {
		//TODO : Get Relative Path and . Ext
		vert = vs;

	}

	//


	return nullptr;
}
