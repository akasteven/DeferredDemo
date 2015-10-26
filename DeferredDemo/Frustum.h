#pragma once

#include "MathUtility.h"
#include <vector>

class Frustum
{
public:
	Frustum();
	~Frustum();

	void static ComputeFrustumCorners(float beg, float end, XMMATRIX & proj, std::vector<XMVECTOR> & corners);

	void static ComputeFrustumFromProjection(Frustum* pOut, XMMATRIX* pProjection);


	XMFLOAT3 Origin;            // Origin of the frustum (and projection).
	XMFLOAT4 Orientation;       // Unit quaternion representing rotation.

	float RightSlope;           // Positive X slope (X/Z).
	float LeftSlope;            // Negative X slope.
	float TopSlope;             // Positive Y slope (Y/Z).
	float BottomSlope;          // Negative Y slope.
	float Near, Far;            // Z of the near plane and far plane.
};
