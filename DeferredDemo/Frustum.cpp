#include "Frustum.h"


Frustum::Frustum()
{
}


Frustum::~Frustum()
{
}

void Frustum::ComputeFrustumFromProjection(Frustum* pOut, XMMATRIX* pProjection)
{
	XMASSERT(pOut);
	XMASSERT(pProjection);

	// Corners of the projection frustum in homogenous space.
	static XMVECTOR HomogenousPoints[6] =
	{
		{ 1.0f, 0.0f, 1.0f, 1.0f },   // right (at far plane)
		{ -1.0f, 0.0f, 1.0f, 1.0f },   // left
		{ 0.0f, 1.0f, 1.0f, 1.0f },   // top
		{ 0.0f, -1.0f, 1.0f, 1.0f },   // bottom

		{ 0.0f, 0.0f, 0.0f, 1.0f },     // near
		{ 0.0f, 0.0f, 1.0f, 1.0f }      // far
	};

	XMVECTOR Determinant;
	XMMATRIX matInverse = XMMatrixInverse(&Determinant, *pProjection);

	// Compute the frustum corners in world space.
	XMVECTOR Points[6];

	for (INT i = 0; i < 6; i++)
	{
		// Transform point.
		Points[i] = XMVector4Transform(HomogenousPoints[i], matInverse);
	}

	pOut->Origin = XMFLOAT3(0.0f, 0.0f, 0.0f);
	pOut->Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	// Compute the slopes.
	Points[0] = Points[0] * XMVectorReciprocal(XMVectorSplatZ(Points[0]));
	Points[1] = Points[1] * XMVectorReciprocal(XMVectorSplatZ(Points[1]));
	Points[2] = Points[2] * XMVectorReciprocal(XMVectorSplatZ(Points[2]));
	Points[3] = Points[3] * XMVectorReciprocal(XMVectorSplatZ(Points[3]));

	pOut->RightSlope = XMVectorGetX(Points[0]);
	pOut->LeftSlope = XMVectorGetX(Points[1]);
	pOut->TopSlope = XMVectorGetY(Points[2]);
	pOut->BottomSlope = XMVectorGetY(Points[3]);

	// Compute near and far.
	Points[4] = Points[4] * XMVectorReciprocal(XMVectorSplatW(Points[4]));
	Points[5] = Points[5] * XMVectorReciprocal(XMVectorSplatW(Points[5]));

	pOut->Near = XMVectorGetZ(Points[4]);
	pOut->Far = XMVectorGetZ(Points[5]);

	return;
}

void Frustum::ComputeFrustumCorners(float nearZ, float farZ, XMMATRIX & proj, std::vector<XMVECTOR> & corners)
{
	Frustum viewFrust;
	ComputeFrustumFromProjection(&viewFrust, &proj);
	viewFrust.Near = nearZ;
	viewFrust.Far = farZ;

	static const XMVECTORU32 vGrabY = { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 };
	static const XMVECTORU32 vGrabX = { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 };

	XMVECTORF32 vRightTop = { viewFrust.RightSlope, viewFrust.TopSlope, 1.0f, 1.0f };
	XMVECTORF32 vLeftBottom = { viewFrust.LeftSlope, viewFrust.BottomSlope, 1.0f, 1.0f };
	XMVECTORF32 vNear = { viewFrust.Near, viewFrust.Near, viewFrust.Near, 1.0f };
	XMVECTORF32 vFar = { viewFrust.Far, viewFrust.Far, viewFrust.Far, 1.0f };
	XMVECTOR vRightTopNear = XMVectorMultiply(vRightTop, vNear);
	XMVECTOR vRightTopFar = XMVectorMultiply(vRightTop, vFar);
	XMVECTOR vLeftBottomNear = XMVectorMultiply(vLeftBottom, vNear);
	XMVECTOR vLeftBottomFar = XMVectorMultiply(vLeftBottom, vFar);

	corners[0] = vRightTopNear;
	corners[1] = XMVectorSelect(vRightTopNear, vLeftBottomNear, vGrabX);
	corners[2] = vLeftBottomNear;
	corners[3] = XMVectorSelect(vRightTopNear, vLeftBottomNear, vGrabY);

	corners[4] = vRightTopFar;
	corners[5] = XMVectorSelect(vRightTopFar, vLeftBottomFar, vGrabX);
	corners[6] = vLeftBottomFar;
	corners[7] = XMVectorSelect(vRightTopFar, vLeftBottomFar, vGrabY);

}
