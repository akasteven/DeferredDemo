//***************************************************************************************
// RenderStates.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "RenderStates.h"

ID3D11RasterizerState* RenderStates::WireframeRS     = 0;
ID3D11RasterizerState* RenderStates::NoCullRS        = 0;
ID3D11RasterizerState* RenderStates::CullClockwiseRS = 0;
ID3D11RasterizerState* RenderStates::CullCounterClockwiseRS = 0;
ID3D11RasterizerState* RenderStates::ShadowMapDepthRS = 0;

ID3D11BlendState*      RenderStates::AlphaToCoverageBS      = 0;
ID3D11BlendState*      RenderStates::TransparentBS          = 0;
ID3D11BlendState*      RenderStates::NoRenderTargetWritesBS = 0;

ID3D11DepthStencilState* RenderStates::DeferredScreenQuadDSS = 0;


void RenderStates::InitAll(ID3D11Device* device)
{
	// WireframeRS
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;
	HR(device->CreateRasterizerState(&wireframeDesc, &WireframeRS));

	// NoCullRS
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;
	HR(device->CreateRasterizerState(&noCullDesc, &NoCullRS));

	// CullClockwiseRS
	D3D11_RASTERIZER_DESC cullClockwiseDesc;
	ZeroMemory(&cullClockwiseDesc, sizeof(D3D11_RASTERIZER_DESC));
	cullClockwiseDesc.FillMode = D3D11_FILL_SOLID;
	cullClockwiseDesc.CullMode = D3D11_CULL_BACK;
	cullClockwiseDesc.FrontCounterClockwise = true;
	cullClockwiseDesc.DepthClipEnable = true;
	HR(device->CreateRasterizerState(&cullClockwiseDesc, &CullClockwiseRS));

	// CullCounterClockwiseRS
	D3D11_RASTERIZER_DESC cullCounterClockwiseDesc;
	ZeroMemory(&cullCounterClockwiseDesc, sizeof(D3D11_RASTERIZER_DESC));
	cullCounterClockwiseDesc.FillMode = D3D11_FILL_SOLID;
	cullCounterClockwiseDesc.CullMode = D3D11_CULL_BACK;
	cullCounterClockwiseDesc.FrontCounterClockwise = false;
	cullCounterClockwiseDesc.DepthClipEnable = true;
	HR(device->CreateRasterizerState(&cullCounterClockwiseDesc, &CullCounterClockwiseRS));

	//ShadowMapRS
	D3D11_RASTERIZER_DESC shadowMapDepthDesc;
	ZeroMemory(&shadowMapDepthDesc, sizeof(D3D11_RASTERIZER_DESC));
	shadowMapDepthDesc.FillMode = D3D11_FILL_SOLID;
	shadowMapDepthDesc.CullMode = D3D11_CULL_BACK;
	shadowMapDepthDesc.FrontCounterClockwise = true;
	shadowMapDepthDesc.DepthClipEnable = true;
	shadowMapDepthDesc.DepthBias = 10000;
	shadowMapDepthDesc.DepthBiasClamp = 0.0f;
	shadowMapDepthDesc.SlopeScaledDepthBias = 4.0f;
	HR(device->CreateRasterizerState(&shadowMapDepthDesc, &ShadowMapDepthRS));

	// AlphaToCoverageBS
	D3D11_BLEND_DESC alphaToCoverageDesc = {0};
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HR(device->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS));

	// TransparentBS
	D3D11_BLEND_DESC transparentDesc = {0};
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;
	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HR(device->CreateBlendState(&transparentDesc, &TransparentBS));

	// NoRenderTargetWritesBS
	D3D11_BLEND_DESC noRenderTargetWritesDesc = {0};
	noRenderTargetWritesDesc.AlphaToCoverageEnable = false;
	noRenderTargetWritesDesc.IndependentBlendEnable = false;
	noRenderTargetWritesDesc.RenderTarget[0].BlendEnable    = false;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_ONE;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].RenderTargetWriteMask = 0;
	HR(device->CreateBlendState(&noRenderTargetWritesDesc, &NoRenderTargetWritesBS));

	D3D11_DEPTH_STENCIL_DESC deferredScreenQuadPassDesc = { 0 };
	deferredScreenQuadPassDesc.DepthEnable = true;
	deferredScreenQuadPassDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	deferredScreenQuadPassDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	deferredScreenQuadPassDesc.StencilEnable = false;
	HR(device->CreateDepthStencilState(&deferredScreenQuadPassDesc, &DeferredScreenQuadDSS));
}

void RenderStates::DestroyAll()
{
	ReleaseCOM(WireframeRS);
	ReleaseCOM(NoCullRS);
	ReleaseCOM(CullClockwiseRS);
	ReleaseCOM(CullCounterClockwiseRS);
	ReleaseCOM(ShadowMapDepthRS);
	ReleaseCOM(AlphaToCoverageBS);
	ReleaseCOM(TransparentBS);
	ReleaseCOM(NoRenderTargetWritesBS);
	ReleaseCOM(DeferredScreenQuadDSS);
}