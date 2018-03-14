#pragma once

#include <graphics/shader.h>
#include <graphics/dx11/dxconfig.h>

#define VS_ENTRY	"VS"
#define PS_ENTRY	"PS"

#define VS_VER		"vs_5_0"
#define PS_VER		"ps_5_0"

class DXShader : public Shader
{
public:
	DXShader();
	virtual~DXShader();

	void InitFromFile(const char* vs, const char* fs) override;
	void InitFromCode(const char* vs, const char* fs) override;

	void Bind() override;
	void UnBind() override;

	virtual void CreateInputLayout(ID3DBlob* vBlob) {/* do nothing */};

	void CompileFromFile(const std::string& file, const char* entry, const char* ver, ID3DBlob** blob);
	void CompileFromCode(const char* code, const char* entry, const char* ver, ID3DBlob** blob);

	ID3D11VertexShader*		mVShader;
	ID3D11PixelShader*		mPShader;
	//TODO : Remove InputLayout on Shader Side
	ID3D11InputLayout*		mInputLaout;
};