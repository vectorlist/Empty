#pragma once

typedef long HRESULT;			//Forward declared dont use windows.h on header

#undef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCSIG__

#define __FILENAME__ (strrchr(__FILE__,'\\')+1)

#define LOG_ERROR(x)		Log::LogError(LogType::Error, x + std::string("\n") + __PRETTY_FUNCTION__ + "\n"\
 + std::to_string(__LINE__))

#define LOG_FATAL(x)		Log::LogError(LogType::Fatal, x + std::string("\n") + __PRETTY_FUNCTION__ + "\n"\
+ "file : " +  __FILENAME__ + "\n" + "line : " + std::to_string(__LINE__))

#define LOG_HR         Log::gLog(std::string("") +__PRETTY_FUNCTION__ + "\n"\
+ "file : " +  __FILENAME__ + "\n" + "line : " + std::to_string(__LINE__))

#define LOG_GR()		{ \
	auto e = glGetError();    \
	if(e == 0) return;\
	Log::LogError(LogType::Error, Log::GetGLError(e));        \
}

#define LOG		std::cout
#define ENDN	"\n"
#define PAUSE	system("pause")

enum class LogType : unsigned short
{
	Warning,
	Error,
	Fatal
};

class Log
{
public:
	static void LogError(LogType type, const std::string &code);
	static char* LogHRToChar(long err);

	//for directx 11 error
	void operator<<(HRESULT hrCode);

	//TODO : OpenGL, VULKAN Debug << operator

	static const char* GetGLError(unsigned int error);

	Log& operator()(const std::string &code)
	{
		mCodes = code;
		return *this;
	}
	std::string mCodes;

	static Log& GetInstance();
	static Log gLog;
};

