#pragma once

#include "DemoBase.h"

class DemoApp :public DemoBase
{
public:
	DemoApp(HINSTANCE hInstance);
	~DemoApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

private:

	//Common setups
	void CreateGeometry();
	void CreateScreenQuad();
	void CreateShaders();
	void CreateContantBuffers();
	void CreateRenderStates();
	void CreateSamplerStates();
	void SetUpSceneConsts();
	void CreateLights();

	void CreateGBuffer();


private:

	//GBuffer Resource Views and Render Target Views
	ID3D11ShaderResourceView * m_pPositionSRV;
	ID3D11ShaderResourceView * m_pNormalSRV;
	ID3D11ShaderResourceView * m_pAlbedoSRV;

	ID3D11RenderTargetView * m_pPositionRTV;
	ID3D11RenderTargetView * m_pNormalRTV;
	ID3D11RenderTargetView * m_pAlbedoRTV;

	//Shaders
	ID3D11VertexShader * m_pGBufferVS;
	ID3D11PixelShader * m_pGBufferPS;
	ID3D11VertexShader * m_pShadingVS;
	ID3D11PixelShader * m_pShadingPSPoint;
	ID3D11PixelShader * m_pShadingPSSpot;
	ID3D11PixelShader * m_pShadingPSDirectional;

	//Shader Constant Buffers
	ID3D11Buffer * m_pCBNeverChanges;
	ID3D11Buffer * m_pCBOnResize;
	ID3D11Buffer * m_pCBPerFrame;
	ID3D11Buffer * m_pCBPerObject;
	ID3D11Buffer * m_pCBPerLight;

	//Vertex and Index Buffers
	ID3D11Buffer* m_pScreenQuadVB;
	ID3D11Buffer* m_pScreenQuadIB;
	ID3D11Buffer * m_pSphereVB;
	ID3D11Buffer * m_pSphereIB;
	ID3D11Buffer *m_pInstancedBuffer;

	ID3D11Texture2D * m_pPositionTexture;
	ID3D11Texture2D * m_pNormalTexture;
	ID3D11Texture2D * m_pAlbedoTexture;

	//Texture Shader Resources View
	ID3D11ShaderResourceView * m_pSphereSRV; 

	//Sampler States
	ID3D11SamplerState * m_pSampleLinear;

	//Matrices
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProj;

	//Lights
	DirectionalLight mDirLight;
	PointLight mPointLight;

	//Scene Parameter
	UINT mInstanceCnt;
	UINT mIndexCnt;
	
};

