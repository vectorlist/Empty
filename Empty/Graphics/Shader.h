#pragma once

class Shader
{
public:
	Shader() : mIsCompiled(false) {}
	virtual~Shader() {}

	virtual void InitFromFile(const char* vs, const char* fs) = 0;
	virtual void InitFromCode(const char* vs, const char* fs) = 0;

	virtual void Bind() = 0;
	virtual void UnBind() = 0;
protected:
	bool mIsCompiled;
};

