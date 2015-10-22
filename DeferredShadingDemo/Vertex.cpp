#include "Vertex.h"


const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::VertexP[1] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};



const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::VertexPT[2] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::VertexPN[2] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::VertexPNT[3] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::VertexPNTTan[4] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },

};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::VertexPN_INS[6] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
	{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
};


const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::VertexPNT_INS[7] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
};

ID3D11InputLayout* InputLayouts::VertexP = 0;
ID3D11InputLayout* InputLayouts::VertexPT = 0;
ID3D11InputLayout* InputLayouts::VertexPNT = 0;
ID3D11InputLayout * InputLayouts::VertexPNTTan = 0;
ID3D11InputLayout * InputLayouts::VertexPN_INS = 0;
ID3D11InputLayout * InputLayouts::VertexPN = 0;
ID3D11InputLayout * InputLayouts::VertexPNT_INS = 0;

void InputLayouts::InitLayout(ID3D11Device* device, ID3DBlob * blob, Vertex::VERTEXFORMAT format)
{
	switch (format)
	{
	case Vertex::POS:
		HR(device->CreateInputLayout(InputLayoutDesc::VertexP, 1, blob->GetBufferPointer(),
			blob->GetBufferSize(), &VertexP));
		break;
	case Vertex::POSCOL:
		break;
	case Vertex::POSNOR:
		HR(device->CreateInputLayout(InputLayoutDesc::VertexPN, 2, blob->GetBufferPointer(),
			blob->GetBufferSize(), &VertexPN));
		break;
	case Vertex::POSTEX:
		HR(device->CreateInputLayout(InputLayoutDesc::VertexPT, 2, blob->GetBufferPointer(),
			blob->GetBufferSize(), &VertexPT));
		break;
	case Vertex::POSNORTEX:
		HR(device->CreateInputLayout(InputLayoutDesc::VertexPNT, 3, blob->GetBufferPointer(),
			blob->GetBufferSize(), &VertexPNT));
		break;
	case  Vertex::POSNORTEXTAN:
		HR(device->CreateInputLayout(InputLayoutDesc::VertexPNTTan, 4, blob->GetBufferPointer(),
			blob->GetBufferSize(), &VertexPNTTan));
		break;
	case Vertex::POSNOR_INS:
		HR(device->CreateInputLayout(InputLayoutDesc::VertexPN_INS, 6, blob->GetBufferPointer(),
			blob->GetBufferSize(), &VertexPN_INS)); 
		break; 
	case Vertex::POSNORTEX_INS:
		HR(device->CreateInputLayout(InputLayoutDesc::VertexPNT_INS, 7, blob->GetBufferPointer(),
			blob->GetBufferSize(), &VertexPNT_INS));
		break;
	default:
		break;
	}
}

void InputLayouts::DestroyAll()
{
	ReleaseCOM(VertexPT);
	ReleaseCOM(VertexPNT);
	ReleaseCOM(VertexPNTTan);
	ReleaseCOM(VertexPN_INS);
	ReleaseCOM(VertexPN);
	ReleaseCOM(VertexPNT_INS);
}

