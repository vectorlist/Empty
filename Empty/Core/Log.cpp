#include <Core/PCH.h>

#include <Core/Log.h>
#include <Graphics/GL4/GLConfig.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//Log Log::gLog = Log();
Log Log::gLog = Log::GetInstance();

void Log::LogError(LogType type, const std::string& code)
{
	switch (type)
	{
	case LogType::Warning:
		MessageBoxA(NULL, code.c_str(), "Wanring", MB_OK);
		break;
	case LogType::Error:
	{
		auto res = MessageBoxA(NULL, code.c_str(), "Error", MB_OKCANCEL);
		if (res == IDCANCEL) {
			exit(0);
		}
		break;
	}
	case LogType::Fatal:
	{
		MessageBoxA(NULL, code.c_str(), "Fatal", MB_OK);
		exit(0);
		break;
	}
	default:
		break;
	}
}

//HRESULT for DirectX
char* Log::LogHRToChar(long err)
{
	switch (err)
	{
	case S_OK:				return "S_OK";
	case E_ABORT:			return "S_OK";
	case E_ACCESSDENIED:	return "E_ACCESSDENIED";
	case E_FAIL:			return "E_FAIL";
	case E_HANDLE:			return "E_HANDLE";
	case E_INVALIDARG:		return "E_INVALIDARG";
	case E_NOINTERFACE:		return "E_NOINTERFACE";
	case E_OUTOFMEMORY:		return "E_OUTOFMEMORY";
	case E_POINTER:			return "E_POINTER";
	case E_UNEXPECTED:		return "E_UNEXPECTED";
	default:				return "UNKOWN";
	}
}

const char* Log::GetGLError(unsigned int error)
{
	switch (error)
	{
	case GL_INVALID_ENUM:      return "Invalid enum";
	case GL_INVALID_VALUE:     return "Invalid value";
	case GL_INVALID_OPERATION: return "Invalid operation";
	case GL_OUT_OF_MEMORY:     return "Out of memory";
	case GL_STACK_OVERFLOW:    return "Stack overflow";
	case GL_STACK_UNDERFLOW:   return "Stack underflow";
	default:                   return "UNKOWN";
	}
}

Log& Log::GetInstance()
{
	static Log log;
	return log;
}

void Log::operator<<(HRESULT hrCode)
{
	if (hrCode == S_OK) return;
	std::string errCode = LogHRToChar(hrCode);
	errCode += "\n" + mCodes;
	MessageBoxA(NULL, errCode.c_str(), "HRESULT ERROR", MB_OK);
}
