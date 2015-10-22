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

private:

	//Shaders
	ID3D11VertexShader * m_pShadingVS;
	ID3D11PixelShader * m_pShadingPS;
	ID3D11VertexShader * m_pGBufferVS;
	ID3D11PixelShader * m_pGBufferPS;

	//Shader Constant Buffers
	ID3D11Buffer * m_pCBNeverChanges;
	ID3D11Buffer * m_pCBOnResize;
	ID3D11Buffer * m_pCBPerFrame;
	ID3D11Buffer * m_pCBPerObject;

	//Vertex and Index Buffers
	ID3D11Buffer* m_pScreenQuadVB;
	ID3D11Buffer* m_pScreenQuadIB;
	ID3D11Buffer * m_pSphereVB;
	ID3D11Buffer * m_pSphereIB;
	ID3D11Buffer *m_pInstancedBuffer;

	//Shader Resources
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
	UINT instanceCnt;
};

