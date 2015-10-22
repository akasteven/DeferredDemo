#pragma once

#include <D3DX11.h>
#include <xnamath.h>
#include "MathUtility.h"
#include "Vertex.h"

class AABB;

class BoundingSphere
{
public:
	BoundingSphere(): Center(0.0f, 0.0f, 0.0f), Radius(0.0f) {}
	BoundingSphere(const XMFLOAT3 & center, const float &radius) :Center(center), Radius(radius) {}
	~BoundingSphere();

	void Clear(){ Center = XMFLOAT3(0.0f, 0.0f, 0.0f); XMFLOAT3(0.0f, 0.0f, 0.0f); }
	void AddVertex(const Vertex::VertexPNTTan &vertex);
	void BuildFromVertices(Vertex::VertexBase * vertex, int numVer);
	void BuildFromAABB(AABB * aabb);

public:

	XMFLOAT3 Center;
	float Radius;
};

