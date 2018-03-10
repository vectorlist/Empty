#pragma once

//#include <vector>
//#include <string>

class FileSystem
{
public:
	static std::vector<std::string> mShaderLangaugeEXTs;

	static std::string GetEXTFromPath(const std::string& path);
};