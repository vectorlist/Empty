#pragma once

#include <graphics/texture.h>

class ID3D11SamplerState;
class DXTexture : public Texture
{
public:
	DXTexture();
	virtual~DXTexture();

	void InitFromFile(const std::string& filename) override;
	void Init(TextureCreateInfo& info);
	void Bind(uint index) override;

	ID3D11ShaderResourceView* mTexture;
	static ID3D11SamplerState* mSampler;
	ID3D11Device*	mDevice;
	ID3D11DeviceContext* mContext;

private:
	static void CreateExternalSampler();
};