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

	void SetSpeed(float speed);

	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetProjMatrix() const;
	XMMATRIX GetViewProjMatrix() const;
	XMFLOAT3 GetPos() const;
	float GetNearZ() const;
	float GetFarZ() const;

private:

	XMFLOAT4X4 m_matView;
	XMFLOAT4X4 m_matProj;
	XMFLOAT4X4 m_matViewProj;

	float fov;
	float aspectRatio;
	float nearZ;
	float farZ;
	float mMoveSpeed;

	XMFLOAT3 mPos;
	XMFLOAT3 mRight;
	XMFLOAT3 mUp;
	XMFLOAT3 mLook;
};

