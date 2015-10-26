#pragma once

#include <D3DX11.h>
#include <xnamath.h>
#include <string>

class Camera;

class SkyBox
{
public:
	SkyBox(ID3D11Device *device,  float radius, UINT sliceCnt, UINT stackCnt, std::string cubeMapFileName);
	~SkyBox();

	void Draw(ID3D11DeviceContext *contex,  Camera *camera);

private:

	struct CBPerFrame
	{
		XMMATRIX worldViewProj;
	} mCBPerFrame;

	ID3D11Buffer * m_pVB;
	ID3D11Buffer * m_pIB;
	ID3D11Buffer * m_pCBPerFrame;

	ID3D11ShaderResourceView * m_pSkySRV;
	ID3D11VertexShader *m_pVS;
	ID3D11PixelShader *m_pPS;

	ID3D11SamplerState * m_pSampleTriLinear;
	ID3D11DepthStencilState * m_pDSS;

	UINT idxCnt;
	float mRadius;
};

