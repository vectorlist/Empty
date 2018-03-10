#include <PCH.h>
#include <core/FileSystem.h>

std::vector<std::string> FileSystem::mShaderLangaugeEXTs = {
	"glsl", "hlsl", "GLSL", "HLSL"
};

std::string FileSystem::GetEXTFromPath(const std::string & path)
{
	int pos = path.rfind('.', path.length());

	if (pos != std::string::npos) {
		auto ext = path.substr(pos + 1, path.length() - pos);
		return ext;
	}
	return "none";
}
