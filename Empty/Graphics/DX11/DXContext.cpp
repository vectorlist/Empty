#include <Core/PCH.h>
#include "DXContext.h"
#include <window/applicationinfo.h>

#include <graphics/dx11/dxtexture.h>

DXContext::DXContext(uint width, uint height, void* hwnd, bool vsync)
	: Context(width, height, hwnd, vsync)
{
	mApiType = GraphicAPI::DIRECTX11;
	Init();
	globalDXDevice = mDevice;
	globalDXDeviceContext = mContext;
}


DXContext::~DXContext()
{
	SAFE_RELEASE(mSwapchain);
	SAFE_RELEASE(mDevice);
	SAFE_RELEASE(mContext);
}

void DXContext::Clear(float r, float g, float b, float a)
{
	float rgba[4] = { r,g,b,a };
	mContext->ClearRenderTargetView(mRenderTargetView, rgba);
	mContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	this->SetMainRenderTargetView();
}


void DXContext::SwapBuffer()
{
	if (mVsync)
		mSwapchain->Present(1, 0);
	else
		mSwapchain->Present(0, 0);
}


void DXContext::SetViewport(const Viewport& viewport)
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = (float)viewport.x;
	vp.TopLeftY = (float)viewport.y;
	vp.Width = (float)viewport.w;
	vp.Height = (float)viewport.h;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;				
	uint vpNum = 1;
	mContext->RSSetViewports(vpNum, &vp);
}

void DXContext::GetViewport(Viewport& viewport)
{
	D3D11_VIEWPORT vp;
	uint vpnum = 1;
	mContext->RSGetViewports(&vpnum, &vp);
	viewport.x = (uint)vp.TopLeftX;
	viewport.y = (uint)vp.TopLeftY;
	viewport.w = (uint)vp.Width;
	viewport.h = (uint)vp.Height;
}

void DXContext::SetBlendState(bool enable)
{
	const float factor[4] = { 0.0f,0.0f,0.0f,0.0f };
	if (enable)
		mContext->OMSetBlendState(state.blendenable, factor, 0xFFFFFFFF);
	else
		mContext->OMSetBlendState(state.blendDisable, factor, 0xFFFFFFFF);
}

void DXContext::SetWireFrameState(bool enable)
{
	if (enable)
		mContext->RSSetState(state.rasterWireFrameState);
	else
		mContext->RSSetState(state.rasterSolidCullBack);
}

void DXContext::SetDepthStencil(bool enable)
{
	if (enable)
		mContext->OMSetDepthStencilState(state.depthStencilEnable, 1);
	else
		mContext->OMSetDepthStencilState(state.depthStencilDisable, 1);
}

void DXContext::SetTexture(uint slot, Texture* texture)
{
	//any cost?
	DXTexture* dst = static_cast<DXTexture*>(texture);
	mContext->PSSetShaderResources(slot, 1, &dst->mTexture);
}

void DXContext::SetDepthStencilEx(DepthStencilState* depthState)
{
	if (depthState->Enabled) {
		switch (depthState->Func)
		{
		case DepthFunc::LESS:
			mContext->OMSetDepthStencilState(depthStates.EnableMaskedAllCompLess, 1);
			break;
		case DepthFunc::LESS_EQUAL:
			mContext->OMSetDepthStencilState(depthStates.EnableMaskedAllCompLessEqual, 1);
			break;
		default:
			break;
		}
	}
	else {
		mContext->OMSetDepthStencilState(depthStates.DisableMaskedNoneCompNever, 1);
	}
}

void DXContext::SetCullMode(CullMode mode)
{
	switch (mode)
	{
	case CullMode::FRONT:
		mContext->RSSetState(state.rasterSolidCullFront);
		break;
	case CullMode::BACK:
		mContext->RSSetState(state.rasterSolidCullBack);
		break;
	case CullMode::FRONT_AND_BACK:
		mContext->RSSetState(state.rasterSolidCullNone);
	case CullMode::NONE:
		mContext->RSSetState(state.rasterSolidCullNone);
		break;
	}
}

void DXContext::SetMainRenderTargetView()
{
	//prepare render target and depth stencil
	mContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	//mContext->OMSetDepthStencilState(mDepthStencilState, 1);
	mContext->RSSetViewports(1, &mViewPort);
}

void DXContext::Init()
{
	CreateDeviceAndContext();
	CreateStates();
}

void DXContext::CreateDeviceAndContext()
{

	DXGI_SWAP_CHAIN_DESC swapchainInfo{};
	swapchainInfo.BufferCount = 1;
	swapchainInfo.BufferDesc.Width = mWidth;
	swapchainInfo.BufferDesc.Height = mHeight;
	swapchainInfo.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//TODO : fix with vsync
	swapchainInfo.BufferDesc.RefreshRate.Numerator = 60;
	swapchainInfo.BufferDesc.RefreshRate.Denominator = 1;

	swapchainInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainInfo.OutputWindow = (HWND)mHwnd;		//render window
	swapchainInfo.SampleDesc.Count = 1;
	swapchainInfo.SampleDesc.Quality = 0;

	bool fullscreen = false;
	if (fullscreen)
		swapchainInfo.Windowed = false;
	else
		swapchainInfo.Windowed = true;

	swapchainInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchainInfo.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapchainInfo.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapchainInfo.Flags = 0;

	D3D_FEATURE_LEVEL featureLvl;
	featureLvl = D3D_FEATURE_LEVEL_11_0;

	LOG_HR << D3D11CreateDeviceAndSwapChain(
		NULL,								//adaptor
		D3D_DRIVER_TYPE_HARDWARE,			//driver type
		NULL,								//software
		0,									//flags
		&featureLvl,						//feature level
		1,									//feature levels
		D3D11_SDK_VERSION,					//sdk ver
		&swapchainInfo,						//swapchain info
		&mSwapchain,						//swapchain pointer
		&mDevice,							//device
		NULL,								//feature level
		&mContext);							//context

}

void DXContext::CreateStates()
{
	auto info = GetSubSystem<ApplicationInfo>();

	ID3D11Texture2D* backBuffer;
	//create back buffer
	LOG_HR << mSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

	//Create Render Target View
	LOG_HR << mDevice->CreateRenderTargetView(backBuffer, NULL, &mRenderTargetView);

	//we dont need back buffer anymore
	SAFE_RELEASE(backBuffer);

	D3D11_TEXTURE2D_DESC depthBufferInfo{};
	depthBufferInfo.Width = info->width;
	depthBufferInfo.Height = info->height;
	depthBufferInfo.MipLevels = 1;
	depthBufferInfo.ArraySize = 1;
	depthBufferInfo.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferInfo.SampleDesc.Count = 1;
	depthBufferInfo.SampleDesc.Quality = 0;
	depthBufferInfo.Usage = D3D11_USAGE_DEFAULT;
	depthBufferInfo.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferInfo.CPUAccessFlags = 0;			//dont allow to access cpu
	depthBufferInfo.MiscFlags = 0;

	LOG_HR << mDevice->CreateTexture2D(&depthBufferInfo, NULL, &mDepthStencilBuffer);

	D3D11_DEPTH_STENCIL_DESC depthStencilInfo{};
	depthStencilInfo.DepthEnable = true;
	depthStencilInfo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilInfo.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilInfo.StencilEnable = true;
	depthStencilInfo.StencilWriteMask = 0xFF;
	depthStencilInfo.StencilReadMask = 0xFF;
	
	//front face
	depthStencilInfo.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilInfo.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilInfo.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilInfo.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//back face
	depthStencilInfo.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilInfo.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;	//allow to decrese
	depthStencilInfo.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilInfo.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	LOG_HR << mDevice->CreateDepthStencilState(&depthStencilInfo, &state.depthStencilEnable);

	depthStencilInfo.DepthEnable = false;

	LOG_HR << mDevice->CreateDepthStencilState(&depthStencilInfo, &state.depthStencilDisable);

	mContext->OMSetDepthStencilState(state.depthStencilEnable, 1);

	//create depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewInfo{};
	depthStencilViewInfo.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewInfo.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewInfo.Texture2D.MipSlice = 0;

	LOG_HR <<
		mDevice->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewInfo, &mDepthStencilView);

	//ok far now assign render target
	mContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	//Raster state
	D3D11_RASTERIZER_DESC rasterInfo{};

	rasterInfo.AntialiasedLineEnable = false;
	rasterInfo.CullMode = D3D11_CULL_BACK;			
	rasterInfo.FrontCounterClockwise = false;		
	rasterInfo.DepthBias = 0;
	rasterInfo.DepthBiasClamp = 0.0f;
	rasterInfo.DepthClipEnable = true;
	rasterInfo.FillMode = D3D11_FILL_SOLID;
	rasterInfo.MultisampleEnable = false;
	rasterInfo.ScissorEnable = false;
	rasterInfo.SlopeScaledDepthBias = 0.0f;

	LOG_HR << mDevice->CreateRasterizerState(&rasterInfo, &state.rasterSolidCullBack);

	rasterInfo.CullMode = D3D11_CULL_NONE;			

	LOG_HR << mDevice->CreateRasterizerState(&rasterInfo, &state.rasterSolidCullNone);

	rasterInfo.CullMode = D3D11_CULL_FRONT;

	LOG_HR << mDevice->CreateRasterizerState(&rasterInfo, &state.rasterSolidCullFront);

	rasterInfo.CullMode = D3D11_CULL_BACK;			
	rasterInfo.FillMode = D3D11_FILL_WIREFRAME;

	LOG_HR << mDevice->CreateRasterizerState(&rasterInfo, &state.rasterWireFrameState);

	//set raster default as solid
	mContext->RSSetState(state.rasterSolidCullBack);

	/*------------------------------ view port --------------------------------*/
	mViewPort.Width = (float)info->width;
	mViewPort.Height = (float)info->height;
	mViewPort.MinDepth = 0.0f;				//NDC to View -1 to 1 > 0 to 1
	mViewPort.MaxDepth = 1.0f;
	mViewPort.TopLeftX = 0.0f;				//Top Left   0 0 starting point as screen
	mViewPort.TopLeftY = 0.0f;

	//set to deualt main view port
	mContext->RSSetViewports(1, &mViewPort);

	/*------------------------------ blend state --------------------------------*/

	D3D11_BLEND_DESC blendInfo{};
	blendInfo.RenderTarget[0].BlendEnable = true;
	blendInfo.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendInfo.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendInfo.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendInfo.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendInfo.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendInfo.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendInfo.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	LOG_HR << mDevice->CreateBlendState(&blendInfo, &state.blendenable);

	blendInfo.RenderTarget[0].BlendEnable = false;
	LOG_HR << mDevice->CreateBlendState(&blendInfo, &state.blendDisable);

	/*---------------------------- main sampler state ----------------------------*/
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

	LOG_HR << mDevice->CreateSamplerState(&samplerInfo, &state.samplerWarpState);

	samplerInfo.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerInfo.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerInfo.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	LOG_HR << mDevice->CreateSamplerState(&samplerInfo, &state.samplerClampState);

	//Set Default Sampler(Warp)
	mContext->PSSetSamplers(0, 1, &state.samplerWarpState);
	//==================== Pre Built States ===========================


	this->CreatePreBuiltStates();
}

void DXContext::CreatePreBuiltStates()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilInfo{};
	depthStencilInfo.DepthEnable = true;
	depthStencilInfo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilInfo.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilInfo.StencilEnable = true;
	depthStencilInfo.StencilWriteMask = 0xFF;
	depthStencilInfo.StencilReadMask = 0xFF;

	depthStencilInfo.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilInfo.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilInfo.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilInfo.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilInfo.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilInfo.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;	
	depthStencilInfo.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilInfo.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	LOG_HR << mDevice->CreateDepthStencilState(&depthStencilInfo, &depthStates.EnableMaskedAllCompLess);

	depthStencilInfo.DepthEnable = true;
	depthStencilInfo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilInfo.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	depthStencilInfo.StencilEnable = true;
	depthStencilInfo.StencilWriteMask = 0xFF;
	depthStencilInfo.StencilReadMask = 0xFF;

	LOG_HR << mDevice->CreateDepthStencilState(&depthStencilInfo, &depthStates.EnableMaskedAllCompLessEqual);

	depthStencilInfo.DepthEnable = false;
	depthStencilInfo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilInfo.DepthFunc = D3D11_COMPARISON_NEVER;

	depthStencilInfo.StencilEnable = false;
	depthStencilInfo.StencilWriteMask = 0xFF;
	depthStencilInfo.StencilReadMask = 0xFF;

	LOG_HR << mDevice->CreateDepthStencilState(&depthStencilInfo, &depthStates.DisableMaskedNoneCompNever);


}
