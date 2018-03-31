#pragma once

//#include <vector>
//#include <string>

class FileSystem
{
public:
	static std::string SHADER_DIR;
	static std::string DEFAULT_TEXTURE;
	//static std::vector<std::string> GLSLEXT;
	//static std::vector<std::string> HLSLEXT;

	static std::string GetEXTFromPath(const std::string& path);
	static std::string GetFileNameFromPath(const std::string& path);
	static std::string GetDirectoryFromPath(const std::string& path);

	static std::string ToUpper(const std::string& lower);

	static std::vector<std::string> FileSystem::GetSubDirsInDir(const std::string& dir);
	static std::vector<std::string> FileSystem::GetFilesInDir(const std::string& dir);

	static std::string GetWorkingDir();
	static bool IsFileExist(const std::string& filename);


	//====================== FILE EXTS ===========================
	static const std::vector<std::string>& GetHLSLExtensions();
	static const std::vector<std::string>& GetGLSLExtensions();
 
	static bool IsHLSLFile(const std::string& filaname);
	static bool IsGLSLFile(const std::string& filaname);
	static bool IsSupportedFormat(const std::string& filename, const std::string& format);
};