#include "ShadowMap.h"


ShadowMap::ShadowMap(ID3D11Device *device, UINT width, UINT height)
:mWidth(width), mHeight(height), mDepthDSV(0), mDepthSRV(0), mEnable4xMsaa(0), m4xMsaaQuality(0)
{
	m_matViewport.TopLeftX = 0.0f;
	m_matViewport.TopLeftY = 0.0f;
	m_matViewport.Width = static_cast<float>(width);
	m_matViewport.Height = static_cast<float>(height);
	m_matViewport.MinDepth = 0.0f;
	m_matViewport.MaxDepth = 1.0f;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;

	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D * depthMap = 0;
	HR(device->CreateTexture2D(&texDesc, 0, &depthMap));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	HR(device->CreateDepthStencilView(depthMap, &dsvDesc, &mDepthDSV));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	HR(device->CreateShaderResourceView(depthMap, &srvDesc, &mDepthSRV));

	ReleaseCOM(depthMap);
}


ShadowMap::~ShadowMap()
{
	ReleaseCOM(mDepthSRV);
	ReleaseCOM(mDepthDSV);
}

ID3D11ShaderResourceView * ShadowMap::DepthShaderResourceView()
{
	return mDepthSRV;
}

void ShadowMap::BindShadowMapDSV(ID3D11DeviceContext * context)
{
	context->RSSetViewports(1, &m_matViewport);
	ID3D11RenderTargetView * renderTargets[1] = {0};
	context->OMSetRenderTargets(1, renderTargets, mDepthDSV);
	context->ClearDepthStencilView(mDepthDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}