#include <Core/PCH.h>
#include <Graphics/DX11/DXFrameBuffer.h>
#include <Graphics/Context.h>
#include <Graphics/DX11/DXConfig.h>
#include <Graphics/TypeTransform.h>
#include <Graphics/DX11/DXTexture.h>
#include <Graphics/DX11/DXContext.h>


DXFrameBuffer::DXFrameBuffer()
{
}

DXFrameBuffer::~DXFrameBuffer()
{
}

void DXFrameBuffer::Init(FrameBufferCreateInfo* info)
{
	this->mWidth = info->Width;
	this->mHeight = info->Height;
	this->mType = info->Type;

	//create Texture
	D3D11_TEXTURE2D_DESC texture2dInfo{};
	texture2dInfo.Width = mWidth;
	texture2dInfo.Height = mHeight;
	texture2dInfo.Format = DXTransform::GetFormat(info->BufferFormat);
	texture2dInfo.MipLevels = 1;
	texture2dInfo.ArraySize = 1;
	texture2dInfo.Usage = D3D11_USAGE_DEFAULT;
	texture2dInfo.SampleDesc.Count = 1;
	texture2dInfo.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texture2dInfo.CPUAccessFlags = 0;
	texture2dInfo.MiscFlags = 0;

	mTexture2ds.resize(info->BufferCount);
	for (uint i = 0; i < info->BufferCount; ++i)
	{
		LOG_HR << G_DXDevice->CreateTexture2D(&texture2dInfo, nullptr, &mTexture2ds[i]);
	}

	//create the render targets
	D3D11_RENDER_TARGET_VIEW_DESC rtvInfo{};
	rtvInfo.Format = texture2dInfo.Format;
	rtvInfo.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvInfo.Texture2D.MipSlice = 0;

	mRenderTargetViews.resize(info->BufferCount);

	for (uint i = 0; i < info->BufferCount; ++i)
	{
		LOG_HR << G_DXDevice->CreateRenderTargetView(mTexture2ds[i], &rtvInfo, &mRenderTargetViews[i]);
	}

	//create shader resource view (exactly textures)
	D3D11_SHADER_RESOURCE_VIEW_DESC srvInfo{};
	srvInfo.Format = texture2dInfo.Format;
	srvInfo.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvInfo.Texture2D.MostDetailedMip = 0;
	srvInfo.Texture2D.MipLevels = 1;
	for (uint i = 0; i < info->BufferCount; ++i)
	{
		auto texture = std::make_shared<DXTexture>();
		texture->width = this->mWidth;
		texture->height = this->mHeight;

		LOG_HR << G_DXDevice->CreateShaderResourceView(mTexture2ds[i], &srvInfo,
			&texture->mTexture);

		//add texture to main frame buffer
		mTexture.emplace_back(texture);
	}

	//Create Depth Texture and Rendertaget
	D3D11_TEXTURE2D_DESC depthTextureInfo{};
	depthTextureInfo.Width = mWidth;
	depthTextureInfo.Height = mHeight;
	depthTextureInfo.MipLevels = 1;
	depthTextureInfo.ArraySize = 1;
	depthTextureInfo.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureInfo.SampleDesc.Count = 1;
	depthTextureInfo.SampleDesc.Quality = 0;
	depthTextureInfo.Usage = D3D11_USAGE_DEFAULT;
	depthTextureInfo.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureInfo.CPUAccessFlags = 0;
	depthTextureInfo.MiscFlags = 0;

	LOG_HR << G_DXDevice->CreateTexture2D(&depthTextureInfo, nullptr, &mDepthTextuer2d);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewInfo{};
	depthViewInfo.Format = depthTextureInfo.Format;
	depthViewInfo.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewInfo.Texture2D.MipSlice = 0;

	LOG_HR << G_DXDevice->CreateDepthStencilView(mDepthTextuer2d, &depthViewInfo, &mDepthStencilView);

	//TODO Set View if we need
	mViewPort.Width = (float)mWidth;
	mViewPort.Height = (float)mHeight;
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;
	mViewPort.TopLeftX = 0.0f;
	mViewPort.TopLeftY = 0.0f;
}

void DXFrameBuffer::Clear(float r, float g, float b, float a)
{
	//TODO Clear Depth and Color if needed

	//clear all render target
	float color[] = { r,g,b,a };
	for (uint i = 0; i < mRenderTargetViews.size(); ++i) {
		G_DXContext->ClearRenderTargetView(mRenderTargetViews[i], color);
	}
	G_DXContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DXFrameBuffer::Bind()
{
	G_DXContext->OMSetRenderTargets(mRenderTargetViews.size(), mRenderTargetViews.data(), mDepthStencilView);
	G_DXContext->RSSetViewports(1, &mViewPort);
}

void DXFrameBuffer::UnBind()
{
}

/*
* Copy Frame Buffer Depth MTR to Main RTV Depth Stencil view
*/
bool DXFrameBuffer::BlitDepthStencilBuffer(uint x, uint y, uint w, uint h)
{
	////dst from main depth view
	//auto* dstDepthView = static_cast<ID3D11DepthStencilView*>(GContext->GetDepthStencilView());
	//auto* dstDepthTexture = static_cast<ID3D11Texture2D*>(GContext->GetDepthStencilTexture());
	////DISCARD
	//SAFE_RELEASE(dstDepthView);
	//SAFE_RELEASE(dstDepthTexture);
	//
	////Re Create Render Target View
	//D3D11_TEXTURE2D_DESC depthTextureInfo{};
	//depthTextureInfo.Width = w;
	//depthTextureInfo.Height = h;
	//depthTextureInfo.MipLevels = 1;
	//depthTextureInfo.ArraySize = 1;
	//depthTextureInfo.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //static
	//depthTextureInfo.SampleDesc.Count = 1;
	//depthTextureInfo.SampleDesc.Quality = 0;
	//depthTextureInfo.Usage = D3D11_USAGE_DEFAULT;
	//depthTextureInfo.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//depthTextureInfo.CPUAccessFlags = 0;
	//depthTextureInfo.MiscFlags = 0;

	//HRESULT res;
	//res = G_DXDevice->CreateTexture2D(&depthTextureInfo, nullptr, &dstDepthTexture);
	//if (FAILED(res)) {
	//	return false;
	//}
	//D3D11_DEPTH_STENCIL_VIEW_DESC depthViewInfo{};
	//depthViewInfo.Format = depthTextureInfo.Format;
	//depthViewInfo.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//depthViewInfo.Texture2D.MipSlice = 0;

	//res << G_DXDevice->CreateDepthStencilView(dstDepthTexture, &depthViewInfo, &dstDepthView);
	//
	//if (FAILED(res)) {
	//	return false;
	//}
	////FRAME BUFFER DEPTH TO MAIN
	//G_DXContext->CopyResource(dstDepthTexture, mDepthTextuer2d);	//recorded texture

	////SET MAIN 
	//auto* mainRenderTarget = static_cast<ID3D11RenderTargetView*>(GContext->GetRenderTargetView());

	//DXContext* context = static_cast<DXContext*>(GContext);
	//context->mDepthStencilBuffer = dstDepthTexture;
	//context->mDepthStencilView = dstDepthView;
	//context->mRenderTargetView = mainRenderTarget;

	//G_DXContext->OMSetRenderTargets(1, &mainRenderTarget, dstDepthView);

	//return true;




																	//SET MAIN 
	auto* mainRenderTarget = static_cast<ID3D11RenderTargetView*>(GContext->GetRenderTargetView());


	G_DXContext->OMSetRenderTargets(1, &mainRenderTarget, mDepthStencilView);

	return true;
}
