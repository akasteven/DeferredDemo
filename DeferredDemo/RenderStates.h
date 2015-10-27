//***************************************************************************************
// RenderStates.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Defines render state objects.  
//***************************************************************************************

#pragma once

#include "D3DUtil.h"

class RenderStates
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	// Rasterizer states
	static ID3D11RasterizerState* WireframeRS;
	static ID3D11RasterizerState* NoCullRS;
	static ID3D11RasterizerState* CullClockwiseRS;
	static ID3D11RasterizerState* CullCounterClockwiseRS;
	static ID3D11RasterizerState * ShadowMapDepthRS;

	// Blend states
	static ID3D11BlendState* AlphaToCoverageBS;
	static ID3D11BlendState* TransparentBS;
	static ID3D11BlendState* NoRenderTargetWritesBS;

	static ID3D11BlendState* DeferredScreenQuadBS;
	// Depth/stencil states
	static ID3D11DepthStencilState* DeferredScreenQuadDSS ;
};
