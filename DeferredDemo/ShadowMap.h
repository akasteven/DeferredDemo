#pragma once

#include "D3DUtil.h"

class ShadowMap
{
public:
	ShadowMap(ID3D11Device *device, UINT width, UINT height);
	~ShadowMap();

	ID3D11ShaderResourceView * DepthShaderResourceView();
	void BindShadowMapDSV( ID3D11DeviceContext * context);

private:
	
	UINT mWidth;
	UINT mHeight;

	bool mEnable4xMsaa;
	UINT m4xMsaaQuality;

	ID3D11ShaderResourceView *mDepthSRV;
	ID3D11DepthStencilView * mDepthDSV;

	D3D11_VIEWPORT mViewport;
};

