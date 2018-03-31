#pragma once

#include <Graphics/FrameBuffer.h>
#include <Graphics/DX11/DXConfig.h>

class DXFrameBuffer : public FrameBuffer
{
public:
	DXFrameBuffer();
	virtual~DXFrameBuffer();

	void Init(FrameBufferCreateInfo* info);
	void Clear(float r, float g, float b, float a);	//TODO OPTIONAL
	void Bind();
	void UnBind();

	bool BlitDepthStencilBuffer(uint x, uint y, uint w, uint h) override;
private:
	//Proxy Texture2D
	std::vector<ID3D11Texture2D*> mTexture2ds;

	//TODO DEPTH RTV
	
	std::vector<ID3D11RenderTargetView*> mRenderTargetViews;
	ID3D11Texture2D*					mDepthTextuer2d;
	ID3D11DepthStencilView*				mDepthStencilView;
	//Seperate RTV, SRV

	//TEST
	D3D11_VIEWPORT						mViewPort;

	//intermediate proxy
	ID3D11Texture2D*					mProxyDepthTexture2d = nullptr;
	ID3D11DepthStencilView*				mProxyDepthStencilView = nullptr;
};

//bool CGraphicsDriverDX11::CreateDepthTexture(UINT nWidth, UINT nHeight, DXGI_FORMAT Format,
//	ID3D11Texture2D** ppDepthStencilTexture, ID3D11DepthStencilView** ppDepthStencilView,
//	ID3D11ShaderResourceView** ppDepthStencilSRV, bool bMultiSample)
//{
//	D3D11_TEXTURE2D_DESC TextureDesc;
//	ZeroMemory(&TextureDesc, sizeof(TextureDesc));
//	TextureDesc.Width = nWidth;
//	TextureDesc.Height = nHeight;
//	TextureDesc.MipLevels = 1;
//	TextureDesc.ArraySize = 1;
//	TextureDesc.Format = Format;
//
//	if (bMultiSample)
//	{
//		TextureDesc.SampleDesc.Count = m_nMultiSampleCount;
//		TextureDesc.SampleDesc.Quality = m_nMultiSampleQuality;
//	}
//	else
//	{
//		TextureDesc.SampleDesc.Count = 1;
//		TextureDesc.SampleDesc.Quality = 0;
//	}
//
//	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
//	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//	TextureDesc.CPUAccessFlags = 0;
//	TextureDesc.MiscFlags = 0;
//
//	HRESULT hr = m_pD3D11Device->CreateTexture2D(&TextureDesc, nullptr, ppDepthStencilTexture);
//
//	if (FAILED(hr))
//	{
//		DebugAssertOnce(UNABLE_TO_CREATE_DEPTHBUFFER_TEXTURE);
//		return false;
//	}
//
//	m_pD3D11Device->CreateDepthStencilView(*ppDepthStencilTexture, nullptr, ppDepthStencilView);
//
//	if (FAILED(hr))
//	{
//		DebugAssertOnce(UNABLE_TO_CREATE_DEPTHBUFFER_VIEW);
//		return false;
//	}
//
//	if (ppDepthStencilSRV)
//	{
//		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
//		ZeroMemory(&SRVDesc, sizeof(SRVDesc));
//
//		SRVDesc.Format = TextureDesc.Format;
//		SRVDesc.Texture2D.MipLevels = TextureDesc.MipLevels;
//		SRVDesc.Texture2D.MostDetailedMip = 0;
//		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//
//		hr = m_pD3D11Device->CreateShaderResourceView(*ppDepthStencilTexture, &SRVDesc, ppDepthStencilSRV);
//
//		if (FAILED(hr))
//		{
//			DebugAssertOnce(UNABLE_TO_CREATE_SHADER_RESOURCE_VIEW);
//			return false;
//		}
//	}
//	return true;
//}
//now I try to make a copy of it :
//
//ResolveSubresource(GetZBufferCopyTexture(), 0,
//	GetDepthStencilBufferTexture(), 0, DXGI_FORMAT_D24_UNORM_S8_UINT);
//ResolveSubresource(GetColorCopyTexture(), 0,
//	]GetBackBuffer(), 0, DXGI_FORMAT_R8G8B8A8_UNORM);
//and also try to copy the copy back to the rendertarget / depthstencil
//
//ResolveSubresource(GetDepthStencilBufferTexture(), 0, 
//	GetZBufferCopyTexture(), 0, DXGI_FORMAT_D24_UNORM_S8_UINT);
//Res