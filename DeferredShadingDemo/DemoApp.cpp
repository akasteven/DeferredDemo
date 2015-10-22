#include "DemoApp.h"
#include "Vertex.h"
#include "RenderStates.h"
#include "ShadowMap.h"
#include "SkyBox.h"
#include "GeoGenerator.h"
#include "ConstantBufferDef.h"
#include "AABB.h"


struct CBNeverChanges
{
	DirectionalLight dirLight;
};

struct CBOnResize
{
	XMMATRIX mProjection;
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
	Material material;
	XMMATRIX matWorldInvTranspose;
	int isInstancing;
	XMFLOAT3 padding;
};


DemoApp::DemoApp(HINSTANCE hInstance)
:DemoBase(hInstance),
m_pShadingVS(0),
m_pShadingPS(0),
m_pGBufferVS(0),
m_pGBufferPS(0),
m_pSphereSRV(0),
m_pCBNeverChanges(0),
m_pCBOnResize(0),
m_pCBPerFrame(0),
m_pCBPerObject(0),
m_pSphereVB(0),
m_pSphereIB(0),
m_pInstancedBuffer(0),
m_pScreenQuadVB(0),
m_pScreenQuadIB(0),
m_pSampleLinear(0),
instanceCnt(1)
{
	mRadius = 100;
	mTheta = float(-0.47f*MathHelper::Pi);
	mPhi = float(0.47f*MathHelper::Pi);
	this->mMainWndCaption = L"Demo";
}

DemoApp::~DemoApp()
{
	md3dImmediateContext->ClearState();
	ReleaseCOM(m_pSphereVB);
	ReleaseCOM(m_pSphereIB);
	ReleaseCOM(m_pScreenQuadVB);
	ReleaseCOM(m_pScreenQuadIB);
	ReleaseCOM(m_pInstancedBuffer);
	ReleaseCOM(m_pCBNeverChanges);
	ReleaseCOM(m_pCBOnResize);
	ReleaseCOM(m_pCBPerFrame);
	ReleaseCOM(m_pCBPerObject);
	ReleaseCOM(m_pSampleLinear);
	ReleaseCOM(m_pShadingVS);
	ReleaseCOM(m_pShadingPS);
	ReleaseCOM(m_pGBufferVS);
	ReleaseCOM(m_pGBufferPS);
	ReleaseCOM(m_pSphereSRV);

	InputLayouts::DestroyAll();
	RenderStates::DestroyAll();
}

void DemoApp::OnResize()
{
	DemoBase::OnResize();
}

void DemoApp::CreateLights()
{
	mDirLight.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mDirLight.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mDirLight.Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	mDirLight.Direction = XMFLOAT3(1.0f, -1.0f, 0.5f);
}

void DemoApp::CreateShaders()
{
	ID3DBlob *pBlob = NULL;

	ReleaseCOM(pBlob);
}

void DemoApp::CreateScreenQuad()
{
	Vertex::VertexBase vertex[4];

	vertex[0] = Vertex::VertexBase(-1.0f, -1.0f, 0.0f);

	vertex[1] = Vertex::VertexBase(-1.0f, +1.0f, 0.0f);

	vertex[2] = Vertex::VertexBase(+1.0f, +1.0f, 0.0f);

	vertex[3] = Vertex::VertexBase(+1.0f, -1.0f, 0.0f);


	DWORD index[6] = 
	{
		0, 2, 1, 0, 3, 2
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
	GeoGenerator::GenSphere(10, 10, 10, sphere);

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
	for (int i = 0; i < instanceCnt; i++)
	{
		trans.mat = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		matWorld.push_back(trans);
	}
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

}

void DemoApp::CreateSamplerStates()
{
	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, L"..//Resources//Grey.jpg", 0, 0, &m_pSphereSRV, 0));

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
	//Set Invariant Constant Buffer
	CBNeverChanges cbNeverChanges;
	cbNeverChanges.dirLight = mDirLight;
	md3dImmediateContext->UpdateSubresource(m_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);
	md3dImmediateContext->VSSetConstantBuffers(0, 1, &m_pCBNeverChanges);
	md3dImmediateContext->PSSetConstantBuffers(0, 1, &m_pCBNeverChanges);

	m_pCamera->Setup(XM_PIDIV4, mClientWidth / (float)mClientHeight, 0.01f, 1000.0f);
	m_pCamera->SetPosition(XMFLOAT3(30.0f, 30.0f, -450.0f));

}

void DemoApp::CreateRenderStates()
{
	RenderStates::InitAll(md3dDevice);
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

	return true;
}

void DemoApp::UpdateScene(float dt)
{
	if (GetAsyncKeyState('W') & 0x8000)
		m_pCamera->MoveForward(10.0f * dt);
	if (GetAsyncKeyState('S') & 0x8000)
		m_pCamera->MoveForward(-10.0f*dt);
	if (GetAsyncKeyState('D') & 0x8000)
		m_pCamera->MoveRight(10.0f *dt);
	if (GetAsyncKeyState('A') & 0x8000)
		m_pCamera->MoveRight(-10.0f *dt);
	if (GetAsyncKeyState('E') & 0x8000)
		m_pCamera->Elevate(10.0f *dt);
	if (GetAsyncKeyState('Q') & 0x8000)
		m_pCamera->Elevate(-10.0f *dt);

	//Update Per Frame Constant Buffer
	CBPerFrame cbPerFrame;
	cbPerFrame.eyePos = m_pCamera->GetPos();
	md3dImmediateContext->UpdateSubresource(m_pCBPerFrame, 0, NULL, &cbPerFrame, 0, 0);
	md3dImmediateContext->VSSetConstantBuffers(2, 1, &m_pCBPerFrame);
	md3dImmediateContext->PSSetConstantBuffers(2, 1, &m_pCBPerFrame);
	mWorld = XMMatrixIdentity();

	m_pCamera->Update();
}

void DemoApp::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);

	//Clear Render Targets
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, clearColor);
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	//Set Rasterizer State
	md3dImmediateContext->RSSetState(RenderStates::CullClockwiseRS);

	//Set Vertex and Index Buffers
	UINT strides[2] = { sizeof(Vertex::VertexPNT), sizeof(Vertex::VertexIns_Mat) };
	UINT offsets[2] = { 0, 0 };
	ID3D11Buffer * buffers[2] = { m_pSphereVB, m_pInstancedBuffer };
	md3dImmediateContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	md3dImmediateContext->IASetIndexBuffer(m_pSphereIB, DXGI_FORMAT_R32_UINT, 0);



	HR(mSwapChain->Present(0, 0));
}


