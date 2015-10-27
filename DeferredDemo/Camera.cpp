#include "Camera.h"


Camera::Camera() 
:mPos(0.0f, 0.0f, 0.0f),
mRight(1.0f, 0.0f, 0.0f),
mUp(0.0f, 1.0f, 0.0f),
mLook(0.0f, 0.0f, 1.0f),
mMoveSpeed(10.0f)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(const XMFLOAT3 & p)
{
	mPos = p;
}

XMFLOAT3 Camera::GetPos() const
{
	return mPos;
}

void Camera::MoveForward( float x )
{
	XMVECTOR s = XMVectorReplicate(x);
	XMVECTOR t = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPos);
	XMStoreFloat3(&mPos, XMVectorMultiplyAdd(s, t, p));
}

void Camera::MoveRight(float x)
{
	XMVECTOR s = XMVectorReplicate(x);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPos);
	XMStoreFloat3(&mPos, XMVectorMultiplyAdd(s, r, p));
}

void Camera::Elevate(float x)
{
	XMVECTOR s = XMVectorReplicate(x);
	XMVECTOR u = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR p = XMLoadFloat3(&mPos);
	XMStoreFloat3(&mPos, XMVectorMultiplyAdd(s, u, p));
}

void Camera::Pitch(float x)
{
	XMMATRIX rotate = XMMatrixRotationAxis(XMLoadFloat3(&mRight), x);
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), rotate));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), rotate));
}

void Camera::Roll(float x)
{
	XMMATRIX rotate = XMMatrixRotationAxis(XMLoadFloat3(&mLook), x);
	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), rotate));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), rotate));
}

void Camera::Yaw(float x)
{
	XMMATRIX rotate = XMMatrixRotationY(x);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), rotate));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), rotate));
}

void Camera::SetSpeed(float speed)
{
	mMoveSpeed = speed;
}


XMMATRIX Camera::GetViewMatrix() const
{
	return XMLoadFloat4x4(&m_matView);
}

XMMATRIX Camera::GetProjMatrix() const
{
	return XMLoadFloat4x4(&m_matProj);
}

XMMATRIX Camera::GetViewProjMatrix() const
{
	return XMMatrixMultiply(GetViewMatrix(), GetProjMatrix());
}

float Camera::GetNearZ() const
{
	return nearZ;
}

float Camera::GetFarZ() const
{
	return farZ;
}

void Camera::Setup(float fv, float asp, float nz, float fz)
{
	fov = fv;
	aspectRatio = asp;
	nearZ = nz;
	farZ = fz;
	XMMATRIX P = XMMatrixPerspectiveFovLH(fv, asp, nz, fz);
	XMStoreFloat4x4(&m_matProj, P);
}

void Camera::Update()
{
	XMVECTOR p = XMLoadFloat3(&mPos);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR u = XMLoadFloat3(&mUp);
	XMVECTOR t = XMLoadFloat3(&mLook);

	t = XMVector3Normalize(t);
	u = XMVector3Normalize(XMVector3Cross(t, r));
	r = XMVector3Normalize(XMVector3Cross(u, t));

	float x = -XMVectorGetX(XMVector3Dot(p, r));
	float y = -XMVectorGetX(XMVector3Dot(p, u));
	float z = -XMVectorGetX(XMVector3Dot(p, t));

	XMStoreFloat3(&mRight, r);
	XMStoreFloat3(&mUp, u);
	XMStoreFloat3(&mLook, t);

	XMMATRIX  V = XMMATRIX(
		mRight.x, mUp.x, mLook.x, 0.0f,
		mRight.y, mUp.y, mLook.y, 0.0f,
		mRight.z, mUp.z, mLook.z, 0.0f,
		x, y, z, 1.0f );

	XMStoreFloat4x4(&m_matView, V);
}