//***************************************************************************************
// Vertex.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Defines vertex structures and input layouts.
//***************************************************************************************

#pragma once
#include <d3dx11.h>
#include <xnamath.h>
#include "D3DUtil.h"

namespace Vertex
{
	enum VERTEXFORMAT
	{
		POS = 1,
		POSCOL = 2,
		POSNOL = 3,
		POSNOR ,
		POSTEX ,
		POSNORTEX ,
		POSNORTEXTAN ,
		POSNOR_INS ,
		POSNORTEX_INS
	};

	struct VertexBase
	{
	public:
		VertexBase():Pos(0.0f, 0.0f, 0.0f){}
		VertexBase(const XMFLOAT3& p) :Pos(p){}
		VertexBase(float px, float py, float pz) :Pos(px, py, pz){}
		XMFLOAT3 Pos;
	};

	struct VertexPT:VertexBase
	{
		VertexPT() : VertexBase(), Tex(0.0f, 0.0f) {}
		VertexPT(const XMFLOAT3& p, const XMFLOAT2& uv)
			: VertexBase(p), Tex(uv) {}
		VertexPT(float px, float py, float pz, float u, float v)
			: VertexBase(px, py, pz), Tex(u, v) {}
		XMFLOAT2 Tex;
	};

	struct VertexPN :VertexBase
	{
		VertexPN() : VertexBase(), Normal(0.0f, 0.0f, 0.0f) {}
		VertexPN(const XMFLOAT3& p, const XMFLOAT3& n)
			: VertexBase(p), Normal(n) {}
		VertexPN(float px, float py, float pz, float nx, float ny, float nz)
			: VertexBase(px, py, pz), Normal(nx, ny, nz) {}
		XMFLOAT3 Normal;
	};

	struct VertexPNT :VertexPN
	{
		VertexPNT() :VertexPN(), Tex(0.0f, 0.0f) {}
		VertexPNT(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT2& uv)
			: VertexPN(p, n), Tex(uv){}
		VertexPNT(float px, float py, float pz, float nx, float ny, float nz, float u, float v)
			: VertexPN(px, py, pz, nx, ny, nz), Tex(u, v){}

		XMFLOAT2 Tex;
	};

	struct  VertexPNTTan :VertexPNT
	{
		VertexPNTTan() : VertexPNT(), Tan(0.0f, 0.0f, 0.0f) {}
		VertexPNTTan(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT2& uv, const XMFLOAT3& tan)
			: VertexPNT(p, n, uv), Tan(tan) {}
		VertexPNTTan(float px, float py, float pz, float nx, float ny, float nz, float u, float v, float tx, float ty, float tz)
			: VertexPNT(px, py, pz, nx, ny, nz, u, v), Tan(tx, ty, tz) {}

		XMFLOAT3 Tan;
	};

	struct VertexIns_Mat
	{
		XMMATRIX mat;
	};
}

class InputLayoutDesc
{
public:
	// Init like const int A::a[4] = {0, 1, 2, 3}; in .cpp file.
	static const D3D11_INPUT_ELEMENT_DESC VertexP[1];
	static const D3D11_INPUT_ELEMENT_DESC VertexPT[2];
	static const D3D11_INPUT_ELEMENT_DESC VertexPN[2];
	static const D3D11_INPUT_ELEMENT_DESC VertexPNT[3];
	static const D3D11_INPUT_ELEMENT_DESC VertexPNTTan[4];
	static const D3D11_INPUT_ELEMENT_DESC VertexPN_INS[6];
	static const D3D11_INPUT_ELEMENT_DESC VertexPNT_INS[7];
};

class InputLayouts
{
public:
	static void InitLayout(ID3D11Device* device, ID3DBlob * blob, Vertex::VERTEXFORMAT format);
	static void DestroyAll();

	static ID3D11InputLayout *VertexP;
	static ID3D11InputLayout* VertexPT;     
	static ID3D11InputLayout * VertexPN;
	static ID3D11InputLayout* VertexPNT;
	static ID3D11InputLayout * VertexPNTTan;
	static ID3D11InputLayout * VertexPN_INS;
	static ID3D11InputLayout * VertexPNT_INS;
};

