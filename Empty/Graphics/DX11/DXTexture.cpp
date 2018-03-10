#include <PCH.h>
#include "dxtexture.h"
#include <graphics/context.h>
#include <graphics/dx11/dxconfig.h>

#include <graphics/contextdeform.h>

//TODO make External static Sampler
ID3D11SamplerState* DXTexture::mSampler = nullptr;

DXTexture::DXTexture()
{
	//reffer intermediate device and context for directx11
	mDevice = G_DXDevice;
	mContext = G_DXContext;
}

DXTexture::~DXTexture()
{
	SAFE_RELEASE(mTexture);
	//SAFE_RELEASE(mSampler);
}

void DXTexture::InitFromFile(const std::string& filename)
{
	std::wstring wide;
	wide = wide.assign(filename.begin(), filename.end());

	LOG_HR << DirectX::CreateDDSTextureFromFile(mDevice, wide.c_str(), nullptr, &mTexture);

	ID3D11Resource* res = nullptr;
	mTexture->GetResource(&res);

	ID3D11Texture2D* tex = nullptr;
	LOG_HR << res->QueryInterface(&tex);

	D3D11_TEXTURE2D_DESC info{};
	tex->GetDesc(&info);

	width = info.Width;
	height = info.Height;

	SAFE_RELEASE(res);
	SAFE_RELEASE(tex);
	CreateExternalSampler();
}

//byte per pixel only for now
void DXTexture::Init(TextureCreateInfo& info)
{
	this->width = info.width;
	this->height = info.height;
	//Translate desc
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = info.width;
	desc.Height = info.height;
	desc.MipLevels = info.mipLevels;
	desc.ArraySize = 1;
	desc.Format = deform::GetDXFormat(info.format);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	//Data
	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = info.pData;
	data.SysMemPitch = width;
	data.SysMemSlicePitch = width * height;

	//create texture2d
	ID3D11Texture2D* texture2d = nullptr;

	LOG_HR << mDevice->CreateTexture2D(&desc, &data, &texture2d);

	//create directx texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvInfo{};
	srvInfo.Format = desc.Format;
	srvInfo.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//TODO : detailed Mipmap
	srvInfo.Texture2D.MipLevels = 1;
	srvInfo.Texture2D.MostDetailedMip = 0;

	LOG_HR << mDevice->CreateShaderResourceView(texture2d, &srvInfo, &mTexture);

	SAFE_RELEASE(texture2d);

	CreateExternalSampler();
}

void DXTexture::Bind(uint index)
{
	ASSERT_MSG(mSampler != nullptr, "failed to load sampler");
	mContext->PSSetSamplers(index, 1, &mSampler);
	mContext->PSSetShaderResources(index, 1, &mTexture);
}

void DXTexture::CreateExternalSampler()
{
	static bool isExternalSamplerCreated = false;

	if (!isExternalSamplerCreated) {
		D3D11_SAMPLER_DESC samplerInfo{};
		samplerInfo.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerInfo.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerInfo.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerInfo.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerInfo.BorderColor[0] = 0;
		samplerInfo.BorderColor[1] = 0;
		samplerInfo.BorderColor[2] = 0;
		samplerInfo.BorderColor[3] = 0;
		samplerInfo.MinLOD = 0;
		samplerInfo.MaxAnisotropy = D3D11_FLOAT32_MAX;
		samplerInfo.ComparisonFunc = D3D11_COMPARISON_NEVER;

		LOG_HR << G_DXDevice->CreateSamplerState(&samplerInfo, &mSampler);
		isExternalSamplerCreated = true;
	}
}