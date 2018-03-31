#pragma once

#include <graphics/texture.h>

class DXTexture : public Texture
{
public:
	DXTexture();
	virtual~DXTexture();

	void InitFromDDS(const std::string& filename) override;
	void InitFromImage(const std::string& filename)override;
	void Init(TextureCreateInfo& info);
	void Bind(uint index) override;

	ID3D11ShaderResourceView*	mTexture;
	static ID3D11SamplerState*	mSampler;
private:
	static void CreateExternalSampler();
	ID3D11Device*				mDevice;
	ID3D11DeviceContext*		mContext;
};