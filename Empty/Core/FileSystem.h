#pragma once

//#include <vector>
//#include <string>

class FileSystem
{
public:
	static std::string SHADER_DIR;
	
	static std::vector<std::string> GLSLEXT;
	static std::vector<std::string> HLSLEXT;

	static std::string GetEXTFromPath(const std::string& path);
	static bool FileExist(const std::string& filename);
};