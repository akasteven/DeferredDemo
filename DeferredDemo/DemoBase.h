#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <string>
#include "D3DUtil.h"
#include "GameTimer.h"
#include "Light.h"
#include "Camera.h"

class DemoBase
{
public:
	DemoBase(HINSTANCE hInstance);
	virtual ~DemoBase();


	HINSTANCE AppInst()const;
	HWND      MainWnd()const;
	float     AspectRatio()const;


	int RunDemo();
	virtual bool Init();
	virtual void OnResize();
	virtual void UpdateScene(float dt) = 0;
	virtual void DrawScene() = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y);
	virtual void OnMouseMove(WPARAM btnState, int x, int y);
	virtual void OnKeyDown();

protected:

	bool InitMainWindow();
	bool InitDirect3D();
	virtual void CalculateFrameStats() = 0;

protected:

	HINSTANCE mhAppInst;
	HWND      mhMainWnd;
	bool      mAppPaused;
	bool      mMinimized;
	bool      mMaximized;
	bool      mResizing;
	UINT      m4xMsaaQuality;

	GameTimer mTimer;
	Camera *m_pCamera;

	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mScreenViewport;

	std::wstring mMainWndCaption;
	std::wstring mAdditionalMsg;
	D3D_DRIVER_TYPE md3dDriverType;
	int mClientWidth;
	int mClientHeight;
	bool mEnable4xMsaa;

	POINT mLastMousePos;
	float mTheta;
	float mPhi;
	float mRadius;

};

