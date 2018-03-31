#include <Core/PCH.h>
#include <core/FileSystem.h>
#include <locale>
#include <filesystem>

using namespace std::experimental::filesystem;

std::string FileSystem::SHADER_DIR = "../data/shader/";
std::string FileSystem::DEFAULT_TEXTURE = "../data/texture/checker.jpg";

std::string FileSystem::GetEXTFromPath(const std::string& path)
{
	//int pos = path.rfind('.', path.length());

	//if (pos != std::string::npos) {
	//	auto ext = path.substr(pos + 1, path.length() - pos);
	//	return ext;
	//}
	//return "n/a";
	if (path.empty())
	{
		return "n/a";
	}

	size_t lastindex = path.find_last_of(".");
	if (std::string::npos != lastindex)
	{
		// extension with dot included
		return path.substr(lastindex, path.length());
	}

	return "n/a";
}

std::string FileSystem::GetFileNameFromPath(const std::string & path)
{
	auto lastIndex = path.find_last_of("\\/");
	std::string filename = path.substr(lastIndex + 1, path.length());
	return filename;
}

std::string FileSystem::GetDirectoryFromPath(const std::string & path)
{
	auto lastIndex = path.find_last_of("\\/");
	std::string dir = path.substr(0, lastIndex + 1);
	return dir;
}

std::string FileSystem::ToUpper(const std::string & lower)
{
	std::locale loc;
	std::string upper;

	for (auto& ch : lower)
	{
		upper += std::toupper(ch, loc);
	}
	return upper;
}

std::vector<std::string> FileSystem::GetSubDirsInDir(const std::string& dir)
{
	std::vector<std::string> dirs;
	directory_iterator endIter;
	for (directory_iterator iter(dir); iter != endIter; ++iter)
	{
		if (!is_directory(iter->status()))
			continue;
		dirs.emplace_back(iter->path().generic_string());
	}
	return dirs;
}

std::vector<std::string> FileSystem::GetFilesInDir(const std::string& directory)
{
	std::vector<std::string> filePaths;
	directory_iterator endIter;
	for (directory_iterator iter(directory); iter != endIter; ++iter)
	{
		if (!is_regular_file(iter->status()))
			continue;
		filePaths.emplace_back(iter->path().generic_string());
	}
	return filePaths;
}

std::string FileSystem::GetWorkingDir()
{
	return current_path().generic_string() + "/";
}

bool FileSystem::IsFileExist(const std::string& filename)
{
	struct stat buffer;
	return (::stat(filename.c_str(), &buffer) == 0);
}

const std::vector<std::string>& FileSystem::GetHLSLExtensions()
{
	static const std::vector<std::string> hlslExt = { "hlsl", "HLSL", "vsh", "psh" };
	return hlslExt;
}

const std::vector<std::string>& FileSystem::GetGLSLExtensions()
{
	static const std::vector<std::string> glslExt = { "glsl", "GLSL", "vert","frag" };
	return glslExt;
}

bool FileSystem::IsHLSLFile(const std::string& filename)
{
	std::string ext = GetEXTFromPath(filename);

	auto supportedFormats = GetHLSLExtensions();
	for (const auto& format : supportedFormats)
	{
		if (ext == format || ext == ToUpper(format))
			return true;
	}
	return false;
}

bool FileSystem::IsGLSLFile(const std::string& filename)
{
	std::string ext = GetEXTFromPath(filename);

	auto supportedFormats = GetGLSLExtensions();
	for (const auto& format : supportedFormats)
	{
		if (ext == format || ext == ToUpper(format))
			return true;
	}
	return false;
}

bool FileSystem::IsSupportedFormat(const std::string& filename, const std::string& format)
{
	std::string fileExt = GetEXTFromPath(filename);

	if (fileExt == format || fileExt == ToUpper(format)) {
		return true;
	}
	return false;
}


