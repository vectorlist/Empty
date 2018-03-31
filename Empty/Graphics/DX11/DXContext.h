#pragma once

#include <graphics/context.h>
#include <graphics/dx11/dxconfig.h>

#include <vector>

class DXContext : public Context
{
public:
	DXContext(uint width, uint height, void* hwnd, bool vsync);
	virtual~DXContext();
public:
	//==================== Graphics ==========================
	void Clear(float r, float g, float b, float a) override;
	void SwapBuffer() override;

	void SetViewport(const Viewport& viewport) override;
	void GetViewport(Viewport& viewport) override;

	void SetBlendState(bool enable) override;
	void SetDepthStencil(bool enable) override;

	void SetTexture(uint slot, Texture* texture);

	//=================== API Extension ======================

	void SetDepthStencilEx(DepthStencilState* state) override;
	void SetCullMode(CullMode mode) override;
	//================= External =============================

	void SetWireFrameState(bool enable);
	void SetMainRenderTargetView();

	//================= Internal =============================
	void* GetDepthStencilView() override { return mDepthStencilView; }
	void* GetDepthStencilTexture() { return mDepthStencilBuffer; }
	void* GetRenderTargetView() { return mRenderTargetView; }

	//Core
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mContext;
	IDXGISwapChain*			mSwapchain;

	//state and usages
	ID3D11RenderTargetView*		mRenderTargetView;

	ID3D11Texture2D*			mDepthStencilBuffer;
	ID3D11DepthStencilView*		mDepthStencilView;

	D3D11_VIEWPORT				mViewPort;

	//state
	struct
	{
		ID3D11DepthStencilState*	depthStencilEnable;
		ID3D11DepthStencilState*    depthStencilDisable;
		//raster
		ID3D11RasterizerState*		rasterSolidCullBack;		//Default
		ID3D11RasterizerState*		rasterSolidCullFront;		//Default
		ID3D11RasterizerState*		rasterSolidCullNone;
		
		ID3D11RasterizerState*		rasterWireFrameState;

		ID3D11BlendState*			blendenable;
		ID3D11BlendState*			blendDisable;

		ID3D11SamplerState*			samplerWarpState;		//for any of texture
		ID3D11SamplerState*			samplerClampState;		//for any of texture
	}state;

	struct 
	{
		ID3D11DepthStencilState*	EnableMaskedAllCompLess;
		ID3D11DepthStencilState*	EnableMaskedAllCompLessEqual;

		ID3D11DepthStencilState*	DisableMaskedNoneCompNever;

		//Option Raster


	}depthStates;
	ID3D11DepthStencilState* mDepthStates[12];
private:
	void Init();

	void CreateDeviceAndContext();
	void CreateStates();

	void CreatePreBuiltStates();
};

#define DEPTH_STATE_ENABLE_MASK_ZERO_