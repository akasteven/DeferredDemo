#pragma once

#include <D3DX11.h>
#include <xnamath.h>
#include "MathUtility.h"
#include "Vertex.h"
#include <vector>

class AABB
{
public:

	AABB() :mCenter(0.0f, 0.0f, 0.0f), mExtents(0.0f, 0.0f, 0.0f), mMins(0.0f, 0.0f, 0.0f), mMaxs(0.0f, 0.0f, 0.0f){}
	AABB(const XMFLOAT3 &center, const XMFLOAT3 &ext) :mCenter(center), mExtents(ext){ ComputeMinMax(); }
	~AABB();

	void Clear(){ mMins = XMFLOAT3(0.0f, 0.0f, 0.0f); mMaxs = XMFLOAT3(0.0f, 0.0f, 0.0f); }
	void BuildFromVertices(Vertex::VertexBase * vertex, int numVer);
	void BuildFromVertices(std::vector<XMVECTOR> vertices);


	void AddVertex(const Vertex::VertexBase & vertex);

	float GetRadius(){ return sqrtf(mExtents.x * mExtents.x + mExtents.y * mExtents.y + mExtents.z * mExtents.z); }
	void GetCorners(std::vector<XMVECTOR>& corners);
	
	void ComputeCenterExt();
	void ComputeMinMax();

public:				

	XMFLOAT3 mCenter;
	XMFLOAT3 mExtents;
	XMFLOAT3 mMins;
	XMFLOAT3 mMaxs;
};

