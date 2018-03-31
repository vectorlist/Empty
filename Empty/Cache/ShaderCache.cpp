#include <Core/PCH.h>
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

	switch (GContext->GetApiType())
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

	switch (GContext->GetApiType())
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

	switch (GContext->GetApiType())
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

#define WIN32_MEAN_AND_LEAN
#include <Windows.h>

Shader* ShaderCache::CreateShader(const std::string& vs, const std::string& fs)
{
	//TODO : Check Exists File already
	std::string VS;
	std::string PS;
	//check what is current API
	std::shared_ptr<Shader> shader = nullptr;
	switch (GContext->GetApiType())
	{
	case GraphicAPI::OPENGL45:
	{
		VS += FileSystem::SHADER_DIR + vs + ".glsl";
		if (!FileSystem::IsFileExist(VS)) {
			ASSERT_MSG(0, (std::string("failed to find glsl file") + VS).c_str());
		}
		PS += FileSystem::SHADER_DIR + fs + ".glsl";
		if (!FileSystem::IsFileExist(PS)) {
			ASSERT_MSG(0, (std::string("failed to find glsl file") + VS).c_str());
		}

		auto found = mExternalShader.find(VS + PS);
		if (found != mExternalShader.end()) {
			return found->second.get();
		}

		shader = std::make_shared<GLShader>();
		shader->InitFromFile(VS.c_str(), PS.c_str());
		mExternalShader.insert(std::make_pair(VS + PS, shader));
		break;
	}
	
	case GraphicAPI::DIRECTX11:
	{
		VS += FileSystem::SHADER_DIR + vs + ".hlsl";
		if (!FileSystem::IsFileExist(VS)) {
			ASSERT_MSG(0, (std::string("failed to find hlsl file :") + VS).c_str());
		}
		PS += FileSystem::SHADER_DIR + fs + ".hlsl";
		if (!FileSystem::IsFileExist(PS)) {
			ASSERT_MSG(0, (std::string("failed to find hlsl file : ") + VS).c_str());
		}

		auto found = mExternalShader.find(VS + PS);
		if (found != mExternalShader.end()) {
			return found->second.get();
		}

		shader = std::make_shared<DXShader>();
		shader->InitFromFile(VS.c_str(), PS.c_str());
		mExternalShader.insert(std::make_pair(VS + PS, shader));
		break;
	}
	}
	

	return shader.get();
}
