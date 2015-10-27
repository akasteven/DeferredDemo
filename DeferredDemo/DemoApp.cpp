#include "DemoApp.h"
#include "Vertex.h"
#include "RenderStates.h"
#include "ShadowMap.h"
#include "SkyBox.h"
#include "GeoGenerator.h"
#include "AABB.h"


struct CBNeverChanges
{
	XMFLOAT4 padding;
};

struct CBOnResize
{
	XMMATRIX m_matProjection;
};

struct CBPerFrame
{
	XMFLOAT3 eyePos;
	float pad;
};

struct CBPerObject
{
	XMMATRIX matWorld;
	XMMATRIX matWVP;
	XMMATRIX matWorldInvTranspose;
	int isInstancing;
	float specularPower;
	XMFLOAT2 padding;
};

struct CBPerLight
{
	XMFLOAT3 LightDir;
	float  LightRange;
	XMFLOAT3 LightColor;
	float SpotAngle;
	XMFLOAT3 LightPos;
	float padding;
};


DemoApp::DemoApp(HINSTANCE hInstance)
:DemoBase(hInstance),
m_bPointLightSwitch(true),
m_bDirLightSwitch(true),
m_pShadingVS(0),
m_pShadingPSPoint(0),
m_pShadingPSSpot(0),
m_pShadingPSDirectional(0),
m_pGBufferVS(0),
m_pGBufferPS(0),
m_pSphereSRV(0),
m_pCBNeverChanges(0),
m_pCBOnResize(0),
m_pCBPerFrame(0),
m_pCBPerObject(0),
m_pCBPerLight(0),
m_pPositionTexture(0),
m_pNormalTexture(0),
m_pAlbedoTexture(0),
m_pSphereVB(0),
m_pSphereIB(0),
m_pInstancedBuffer(0),
m_pScreenQuadVB(0),
m_pScreenQuadIB(0),
m_pSampleLinear(0),
m_InstanceCnt(5),
m_IndexCnt(0),
m_LightCnt(0)
{
	mRadius = 100;
	mTheta = float(-0.47f*MathHelper::Pi);
	mPhi = float(0.47f*MathHelper::Pi);
	this->mMainWndCaption = L"Deferred Rendering";
}

DemoApp::~DemoApp()
{
	md3dImmediateContext->ClearState();
	ReleaseCOM(m_pPositionTexture);
	ReleaseCOM(m_pNormalTexture);
	ReleaseCOM(m_pAlbedoTexture);
	ReleaseCOM(m_pSphereVB);
	ReleaseCOM(m_pSphereIB);
	ReleaseCOM(m_pScreenQuadVB);
	ReleaseCOM(m_pScreenQuadIB);
	ReleaseCOM(m_pInstancedBuffer);
	ReleaseCOM(m_pCBNeverChanges);
	ReleaseCOM(m_pCBOnResize);
	ReleaseCOM(m_pCBPerFrame);
	ReleaseCOM(m_pCBPerObject);
	ReleaseCOM(m_pCBPerLight);
	ReleaseCOM(m_pSampleLinear);
	ReleaseCOM(m_pShadingVS);
	ReleaseCOM(m_pShadingPSPoint);
	ReleaseCOM(m_pShadingPSSpot);
	ReleaseCOM(m_pShadingPSDirectional);
	ReleaseCOM(m_pGBufferVS);
	ReleaseCOM(m_pGBufferPS);
	ReleaseCOM(m_pSphereSRV);

	InputLayouts::DestroyAll();
	RenderStates::DestroyAll();
}


void DemoApp::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << L"      "
			<< L"Light Counts: "<<m_LightCnt << L"   "
			<< L"Triangle Counts: " << m_IndexCnt / 3 * m_InstanceCnt << L"   "
			<< L"FPS: " << fps ;
		SetWindowText(mhMainWnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}




void DemoApp::OnResize()
{
	DemoBase::OnResize();
}

void DemoApp::CreateLights()
{
	srand((unsigned int)time(0));

	float dis = 50.0f;
	XMFLOAT3 lightPos[14] = 
	{
		//Top lights
		XMFLOAT3(-dis, dis, dis),
		XMFLOAT3(dis, dis, dis),
		XMFLOAT3(0.0f, dis, 0.0f),
		XMFLOAT3(-dis, dis, -dis),
		XMFLOAT3(dis, dis, -dis),


		//Middle Lights 
		XMFLOAT3(0.0f, 0.0f, dis),
		XMFLOAT3(dis, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, -dis),
		XMFLOAT3(-dis, 0.0f, 0.0f),

		//Bottom lights
		XMFLOAT3(-dis, -dis, dis),
		XMFLOAT3(dis, -dis, dis),
		XMFLOAT3(0.0f, -dis, 0.0f),
		XMFLOAT3(-dis, -dis, -dis),
		XMFLOAT3(dis, -dis, -dis)
	};

	for (UINT i = 0; i < 14; i++)
	{
		LightParams lightP;
		lightP.LightColor = XMFLOAT3(MathHelper::RandF(), MathHelper::RandF(), MathHelper::RandF());
		lightP.LightPos = lightPos[i];
		lightP.LightRange = 60;
		m_vPointLights.push_back(lightP);
	}

	LightParams lightD;
	lightD.LightColor = XMFLOAT3(MathHelper::RandF(), MathHelper::RandF(), MathHelper::RandF());
	lightD.LightDir = XMFLOAT3(MathHelper::RandF(), MathHelper::RandF(), MathHelper::RandF());
	m_vDirLights.push_back(lightD);

	m_LightCnt = m_vDirLights.size() + m_vPointLights.size();
}

void DemoApp::CreateShaders()
{
	ID3DBlob *pBlob = NULL;

	HR(LoadShaderBinaryFromFile("Shaders//GBufferVS.fxo", &pBlob));
	HR(md3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pGBufferVS));
	InputLayouts::InitLayout(md3dDevice, pBlob, Vertex::POSNORTEX_INS);

	HR(LoadShaderBinaryFromFile("Shaders//GBufferPS.fxo", &pBlob));
	HR(md3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pGBufferPS));

	HR(LoadShaderBinaryFromFile("Shaders//DeferredVS.fxo", &pBlob));
	HR(md3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pShadingVS));
	InputLayouts::InitLayout(md3dDevice, pBlob, Vertex::POS);

	HR(LoadShaderBinaryFromFile("Shaders//DeferredPSPointLight.fxo", &pBlob));
	HR(md3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pShadingPSPoint));

	HR(LoadShaderBinaryFromFile("Shaders//DeferredPSSpotLight.fxo", &pBlob));
	HR(md3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pShadingPSSpot));

	HR(LoadShaderBinaryFromFile("Shaders//DeferredPSDirectionalLight.fxo", &pBlob));
	HR(md3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pShadingPSDirectional));

	ReleaseCOM(pBlob);
}

void DemoApp::CreateScreenQuad()
{
	Vertex::VertexBase vertex[4];

	vertex[0] = Vertex::VertexBase(-1.0f, -1.0f, 0.5f);

	vertex[1] = Vertex::VertexBase(-1.0f, +1.0f, 0.5f);

	vertex[2] = Vertex::VertexBase(+1.0f, +1.0f, 0.5f);

	vertex[3] = Vertex::VertexBase(+1.0f, -1.0f, 0.5f);


	DWORD index[6] = 
	{
		0, 1, 2, 0, 2, 3
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex::VertexBase) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertex[0];
	HR(md3dDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_pScreenQuadVB));

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD)* 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &index[0];
	HR(md3dDevice->CreateBuffer(&indexBufferDesc, &iinitData, &m_pScreenQuadIB));

}

void DemoApp::CreateGeometry()
{
	GeoGenerator::Mesh sphere;
	GeoGenerator::GenSphere(5, 50, 50, sphere);
	m_IndexCnt = sphere.indices.size();

	//Vertex
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(Vertex::VertexPNT) * sphere.vertices.size();
	vertexDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexDesc.CPUAccessFlags = 0;
	vertexDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = &sphere.vertices[0];
	HR(md3dDevice->CreateBuffer(&vertexDesc, &data, &m_pSphereVB));

	//Index
	D3D11_BUFFER_DESC indexDesc;
	ZeroMemory(&indexDesc, sizeof(indexDesc));
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.ByteWidth = sizeof(DWORD)* sphere.indices.size();
	indexDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.MiscFlags = 0;

	data.pSysMem = &sphere.indices[0];
	HR(md3dDevice->CreateBuffer(&indexDesc, &data, &m_pSphereIB));

	//Pillars per instance data
	std::vector<Vertex::VertexIns_Mat> matWorld;
	Vertex::VertexIns_Mat trans;

	float interval = 20;
	float startpos = -1.0f * int ( m_InstanceCnt / 2 ) * interval;

	for (UINT i = 0; i < m_InstanceCnt; i++)
	{
		float row = startpos + i * interval;
		for (UINT j = 0; j < m_InstanceCnt; j++)
		{
			float col = startpos + j * interval;
			for (UINT k = 0; k < m_InstanceCnt ; k++)
			{
				float depth = startpos + k  * interval;
				trans.mat = XMMatrixTranslation(col , row, depth);
				matWorld.push_back(trans);
			}
		}
	}

	m_InstanceCnt = m_InstanceCnt * m_InstanceCnt * m_InstanceCnt;
	vertexDesc.ByteWidth = sizeof(Vertex::VertexIns_Mat) * matWorld.size();
	data.pSysMem = &matWorld[0];
	HR(md3dDevice->CreateBuffer(&vertexDesc, &data, &m_pInstancedBuffer));

	CreateScreenQuad();	
}

void DemoApp::CreateContantBuffers()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	desc.ByteWidth = sizeof(CBNeverChanges);
	HR(md3dDevice->CreateBuffer(&desc, 0, &m_pCBNeverChanges));

	desc.ByteWidth = sizeof(CBOnResize);
	HR(md3dDevice->CreateBuffer(&desc, 0, &m_pCBOnResize));

	desc.ByteWidth = sizeof(CBPerFrame);
	HR(md3dDevice->CreateBuffer(&desc, 0, &m_pCBPerFrame));

	desc.ByteWidth = sizeof(CBPerObject);
	HR(md3dDevice->CreateBuffer(&desc, 0, &m_pCBPerObject));

	desc.ByteWidth = sizeof(LightParams);
	HR(md3dDevice->CreateBuffer(&desc, 0, &m_pCBPerLight));

}

void DemoApp::CreateSamplerStates()
{
	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, L"..//Resources//Ground.jpg", 0, 0, &m_pSphereSRV, 0));

	D3D11_SAMPLER_DESC desc; 
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; 
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(md3dDevice->CreateSamplerState(&desc, &m_pSampleLinear));

}

void DemoApp::SetUpSceneConsts()
{
	m_pCamera->Setup(XM_PIDIV4, mClientWidth / (float)mClientHeight, 0.01f, 1000.0f);
	m_pCamera->SetPosition(XMFLOAT3(100.0f, 100.0f, -100.0f));
	m_pCamera->Yaw( - 1.0f * XM_PIDIV4);
	m_pCamera->Pitch( XM_PI / 5.0f);

}

void DemoApp::CreateRenderStates()
{
	RenderStates::InitAll(md3dDevice);
}

void DemoApp::CreateGBuffer()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mClientWidth;
	texDesc.Height = mClientHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format =  DXGI_FORMAT_R32G32B32A32_FLOAT;

	//Check MSAA configuration
	if (mEnable4xMsaa)
	{
		texDesc.SampleDesc.Count = 4;
		texDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
	}

	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	HR(md3dDevice->CreateTexture2D(&texDesc, NULL, &m_pPositionTexture));
	HR(md3dDevice->CreateTexture2D(&texDesc, NULL, &m_pNormalTexture));
	HR(md3dDevice->CreateTexture2D(&texDesc, 0, &m_pAlbedoTexture));

	HR(md3dDevice->CreateShaderResourceView(m_pPositionTexture, 0, &m_pPositionSRV));
	HR(md3dDevice->CreateRenderTargetView(m_pPositionTexture, 0, &m_pPositionRTV));

	HR(md3dDevice->CreateShaderResourceView(m_pNormalTexture, 0, &m_pNormalSRV));
	HR(md3dDevice->CreateRenderTargetView(m_pNormalTexture, 0, &m_pNormalRTV));

	HR(md3dDevice->CreateShaderResourceView(m_pAlbedoTexture, 0, &m_pAlbedoSRV));
	HR(md3dDevice->CreateRenderTargetView(m_pAlbedoTexture, 0, &m_pAlbedoRTV));
}

bool DemoApp::Init()
{
	if (!DemoBase::Init())
		return false;

	CreateLights();
	CreateShaders();
	CreateGeometry();
	CreateContantBuffers();
	CreateSamplerStates();
	CreateRenderStates();
	SetUpSceneConsts();
	CreateGBuffer();

	return true;
}

void DemoApp::UpdateScene(float dt)
{
	if (GetAsyncKeyState('W') & 0x8000)
		m_pCamera->MoveForward(30.0f * dt);
	if (GetAsyncKeyState('S') & 0x8000)
		m_pCamera->MoveForward(-30.0f*dt);
	if (GetAsyncKeyState('D') & 0x8000)
		m_pCamera->MoveRight(30.0f *dt);
	if (GetAsyncKeyState('A') & 0x8000)
		m_pCamera->MoveRight(-30.0f *dt);
	if (GetAsyncKeyState('E') & 0x8000)
		m_pCamera->Elevate(30.0f *dt);
	if (GetAsyncKeyState('Q') & 0x8000)
		m_pCamera->Elevate(-30.0f *dt);

	if (GetAsyncKeyState('N') & 0x8000)
		m_bPointLightSwitch = false;
	if (GetAsyncKeyState('M') & 0x8000)
		m_bPointLightSwitch = true;
	if (GetAsyncKeyState('V') & 0x8000)
		m_bDirLightSwitch = false;
	if (GetAsyncKeyState('B') & 0x8000)
		m_bDirLightSwitch = true;


	//Update Per Frame Constant Buffer
	CBPerFrame cbPerFrame;
	cbPerFrame.eyePos = m_pCamera->GetPos();
	md3dImmediateContext->UpdateSubresource(m_pCBPerFrame, 0, NULL, &cbPerFrame, 0, 0);
	md3dImmediateContext->VSSetConstantBuffers(2, 1, &m_pCBPerFrame);
	md3dImmediateContext->PSSetConstantBuffers(2, 1, &m_pCBPerFrame);
	m_matWorld = XMMatrixIdentity();

	m_pCamera->Update();
}

void DemoApp::DrawScene()
{
	float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f };
	float blendFactors[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	//Write to GBuffer
	ID3D11RenderTargetView * GBufferRenderTargets[3] = { m_pPositionRTV, m_pNormalRTV, m_pAlbedoRTV };
	for (UINT i = 0; i < 3; i++)
		md3dImmediateContext->ClearRenderTargetView(GBufferRenderTargets[i], clearColor);

	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	md3dImmediateContext->OMSetRenderTargets(3, GBufferRenderTargets, mDepthStencilView);
	md3dImmediateContext->OMSetDepthStencilState(NULL, 0x1);
	md3dImmediateContext->OMSetBlendState(NULL, blendFactors, 0xffffffff);

	UINT strides[2] = { sizeof(Vertex::VertexPNT), sizeof(Vertex::VertexIns_Mat) };
	UINT offsets[2] = { 0, 0 };
	ID3D11Buffer * buffers[2] = { m_pSphereVB, m_pInstancedBuffer };
	md3dImmediateContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	md3dImmediateContext->IASetIndexBuffer(m_pSphereIB, DXGI_FORMAT_R32_UINT, 0);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	md3dImmediateContext->IASetInputLayout(InputLayouts::VertexPNT_INS);
	md3dImmediateContext->RSSetState(RenderStates::CullCounterClockwiseRS);
	md3dImmediateContext->VSSetShader(m_pGBufferVS, NULL, 0);
	md3dImmediateContext->PSSetShader(m_pGBufferPS, NULL, 0);

	CBPerObject cbPerObj;
	cbPerObj.isInstancing = 1; 
	cbPerObj.specularPower = 30;
	XMMATRIX world = XMMatrixIdentity();
	cbPerObj.matWorld = XMMatrixTranspose(world);
	cbPerObj.matWorldInvTranspose = XMMatrixTranspose(MathHelper::InverseTranspose(world));
	cbPerObj.matWVP = XMMatrixTranspose(world * m_pCamera->GetViewProjMatrix());
	md3dImmediateContext->UpdateSubresource(m_pCBPerObject, 0, NULL, &cbPerObj, 0, 0);
	md3dImmediateContext->VSSetConstantBuffers(3, 1, &m_pCBPerObject);
	md3dImmediateContext->PSSetConstantBuffers(3, 1, &m_pCBPerObject);
	md3dImmediateContext->PSSetShaderResources(0, 1, &m_pSphereSRV);
	md3dImmediateContext->PSSetSamplers(0, 1, &m_pSampleLinear);

	md3dImmediateContext->DrawIndexedInstanced(m_IndexCnt, m_InstanceCnt , 0, 0, 0 );

	//Shading Screen Quad Pixels
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, clearColor);
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	md3dImmediateContext->OMSetDepthStencilState(RenderStates::DeferredScreenQuadDSS, 0x1);
	md3dImmediateContext->OMSetBlendState(RenderStates::DeferredScreenQuadBS, blendFactors, 0xffffffff);

	UINT stride[1] = { sizeof(Vertex::VertexBase)};
	UINT offset[1] = { 0 };
	ID3D11Buffer * buffer[1] = { m_pScreenQuadVB};
	md3dImmediateContext->IASetVertexBuffers(0, 1, buffer, stride, offset);
	md3dImmediateContext->IASetIndexBuffer(m_pScreenQuadIB, DXGI_FORMAT_R32_UINT, 0);
	md3dImmediateContext->IASetInputLayout(InputLayouts::VertexP);
	md3dImmediateContext->VSSetShader(m_pShadingVS, NULL, 0);

	md3dImmediateContext->PSSetShaderResources(0, 1, &m_pPositionSRV);
	md3dImmediateContext->PSSetShaderResources(1, 1, &m_pNormalSRV);
	md3dImmediateContext->PSSetShaderResources(2, 1, &m_pAlbedoSRV);
	md3dImmediateContext->PSSetConstantBuffers(3, 1, &m_pCBPerLight);

	//Shading Directional Lights
	if (m_bDirLightSwitch)
	{
		md3dImmediateContext->PSSetShader(m_pShadingPSDirectional, NULL, 0);
		for (UINT i = 0; i < m_vDirLights.size(); i++)
		{
			LightParams cbPerlight = m_vDirLights[i];
			md3dImmediateContext->UpdateSubresource(m_pCBPerLight, 0, NULL, &cbPerlight, 0, 0);
			md3dImmediateContext->DrawIndexed(6, 0, 0);
		}
	}

	//Shading Point Lights
	if (m_bPointLightSwitch)
	{
		md3dImmediateContext->PSSetShader(m_pShadingPSPoint, NULL, 0);
		for (UINT j = 0; j < m_vPointLights.size(); j++)
		{
			LightParams cbPerlight = m_vPointLights[j];
			md3dImmediateContext->UpdateSubresource(m_pCBPerLight, 0, NULL, &cbPerlight, 0, 0);
			md3dImmediateContext->DrawIndexed(6, 0, 0);
		}
	}

	HR(mSwapChain->Present(0, 0));
}


