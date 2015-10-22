//***************************************************************************************
// d3dUtil.h by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#ifndef D3DUTIL_H
#define D3DUTIL_H

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
 
#include <d3dx11.h>
#include <xnamath.h>
#include <dxerr.h>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <istream>
#include <d3dcompiler.h>
#include "MathUtility.h"
#include "Light.h"
#include "Vertex.h"
#include <cassert>

class AABB;

//---------------------------------------------------------------------------------------
// Simple d3d error checker for book demos.
//---------------------------------------------------------------------------------------

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif 


//---------------------------------------------------------------------------------------
// Convenience macro for releasing COM objects.
//---------------------------------------------------------------------------------------

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

//---------------------------------------------------------------------------------------
// Convenience macro for deleting objects.
//---------------------------------------------------------------------------------------

#define SafeDelete(x) { delete x; x = 0; }

HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

HRESULT LoadShaderBinaryFromFile(const std::string& filename, ID3DBlob **pBlob);

bool LoadObjModel(std::wstring filename,
	ID3D11Buffer** vertBuff,
	ID3D11Buffer** indexBuff,
	ID3D11Device *device,
	UINT & verCnt,
	bool bComputeNormals = false,
	bool bAverageNormals = false,
	bool bComputeTangents = false,
	bool bAverageTangents = false
	);

bool LoadModel(
	const std::string filename,
	ID3D11Buffer** vertBuff,
	ID3D11Buffer** indexBuff,
	ID3D11Device *device,
	AABB * aabb,
	UINT & verCnt ,
	UINT &triCnt
	);

#endif // D3DUTIL_H