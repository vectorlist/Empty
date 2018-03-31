#include <Core/PCH.h>
#include <graphics/dx11/dxshader.h>
#include <graphics/context.h>

DXShader::DXShader()
	: mVShader(nullptr), mPShader(nullptr), mInputLaout(nullptr)
{
}

DXShader::~DXShader()
{
	SAFE_RELEASE(mVShader);
	SAFE_RELEASE(mPShader);
	SAFE_RELEASE(mInputLaout);
}

void DXShader::InitFromFile(const char* vs, const char* fs)
{
	ID3DBlob* vb = NULL;
	ID3DBlob* pb = NULL;

	CompileFromFile(vs, VS_ENTRY, VS_VER, &vb);

	LOG_HR << G_DXDevice->CreateVertexShader(
		vb->GetBufferPointer(),
		vb->GetBufferSize(),
		NULL,
		&mVShader);
	
	CompileFromFile(fs, PS_ENTRY, PS_VER, &pb);

	LOG_HR << G_DXDevice->CreatePixelShader(
		pb->GetBufferPointer(),
		pb->GetBufferSize(),
		NULL,
		&mPShader);

	CreateInputLayout(vb);
}

void DXShader::InitFromCode(const char* vs, const char* fs)
{
	ID3DBlob* vb = NULL;
	ID3DBlob* pb = NULL;

	HRESULT res;

	CompileFromCode(vs, VS_ENTRY, VS_VER, &vb);

	LOG_HR << G_DXDevice->CreateVertexShader(
		vb->GetBufferPointer(),
		vb->GetBufferSize(),
		NULL,
		&mVShader);

	CompileFromCode(fs, PS_ENTRY, PS_VER, &pb);

	LOG_HR << G_DXDevice->CreatePixelShader(
		pb->GetBufferPointer(),
		pb->GetBufferSize(),
		NULL,
		&mPShader);

	CreateInputLayout(vb);
}

void DXShader::Bind()
{
	if (mInputLaout) {
		G_DXContext->IASetInputLayout(mInputLaout);
	}
	G_DXContext->VSSetShader(mVShader, 0, 0);
	G_DXContext->PSSetShader(mPShader, 0, 0);
}

void DXShader::UnBind()
{
}

void DXShader::CompileFromFile(const std::string& file, const char* entry, const char* ver, ID3DBlob** blob)
{
	ID3DBlob* errBlob = nullptr;
	HRESULT res;

	std::wstring widefile;
	widefile.assign(file.begin(), file.end());

	res = D3DCompileFromFile(widefile.c_str(), nullptr, nullptr,
		entry, ver, 0, 0, blob, &errBlob);

	if (FAILED(res))
	{
		if (!errBlob) {
			MessageBoxA(NULL, file.c_str(), "failed no find filename", MB_OK);
			return;
		}
		MessageBoxA(NULL, (char*)errBlob->GetBufferPointer(), file.c_str(), MB_OK);
	}
}

void DXShader::CompileFromCode(const char* code, const char* entry, const char* ver, ID3DBlob** blob)
{
	ID3DBlob* errBlob = nullptr;

	HRESULT res = D3DCompile(code, strlen(code), nullptr, nullptr, nullptr, entry, ver, 0,
		0, blob, &errBlob);

	if (FAILED(res))
	{
		if (!errBlob) {
			MessageBoxA(NULL, code, "failed no find filename", MB_OK);
			return;
		}
		MessageBoxA(NULL, (char*)errBlob->GetBufferPointer(), code, MB_OK);
	}
}