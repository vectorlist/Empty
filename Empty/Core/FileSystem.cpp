#include <PCH.h>
#include <core/FileSystem.h>

static std::string SHADER_DIR = "../data/shader/";

std::vector<std::string> FileSystem::GLSLEXT = {
	"glsl", "GLSL", "vert","frag"
};

std::vector<std::string> FileSystem::HLSLEXT = {
	"hlsl", "HLSL", "vs", "ps"
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

bool FileSystem::FileExist(const std::string& filename)
{
	struct stat buffer;
	return (::stat(filename.c_str(), &buffer) == 0);
}

