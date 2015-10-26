#pragma once

#include <d3dx11.h>
#include <xnamath.h>

class Camera
{
public:
	Camera();
	~Camera();

	//Camera setups
	void Setup(float fov, float asp, float nz, float fz);
	void Update();
	void SetPosition( const XMFLOAT3 & p);

	//Camera movements
	void MoveForward( float x );
	void MoveRight(float x);
	void Elevate(float x);

	void Pitch(float x);
	void Roll(float x);
	void Yaw(float x);

	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetProjMatrix() const;
	XMMATRIX GetViewProjMatrix() const;
	XMFLOAT3 GetPos() const;
	float GetNearZ() const;
	float GetFarZ() const;

private:

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
	XMFLOAT4X4 mViewProj;

	float fov;
	float aspectRatio;
	float nearZ;
	float farZ;

	XMFLOAT3 mPos;
	XMFLOAT3 mRight;
	XMFLOAT3 mUp;
	XMFLOAT3 mLook;
};

