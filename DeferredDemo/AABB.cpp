#include "AABB.h"


AABB::~AABB()
{
}

void AABB::BuildFromVertices(Vertex::VertexBase * vertex, int numVer)
{
	Clear();
	if (numVer <= 0)
		return;

	for (int i = 0; i < numVer; i++)
	{
		mMaxs = MathHelper::VectorMax(mMaxs, vertex[i].Pos);
		mMins = MathHelper::VectorMin(mMins, vertex[i].Pos);
	}
	ComputeCenterExt();
}

void AABB::BuildFromVertices(std::vector<XMVECTOR> vertices)
{
	Clear();

	XMVECTOR max = XMLoadFloat3(&mMaxs);
	XMVECTOR min = XMLoadFloat3(&mMins);

	for (int i = 0; i < vertices.size(); i++)
	{
		max = XMVectorMax(max, vertices[i]);
		min = XMVectorMin(min, vertices[i]);
	}

	XMStoreFloat3(&mMaxs, max);
	XMStoreFloat3(&mMins, min);
	ComputeCenterExt();
}

void AABB::AddVertex(const Vertex::VertexBase & vertex)
{
	if (vertex.Pos.x >mMaxs.x)	mMaxs.x = vertex.Pos.x;
	if (vertex.Pos.x <mMins.x)   mMins.x = vertex.Pos.x;

	if (vertex.Pos.y >mMaxs.y)  mMaxs.y = vertex.Pos.y;
	if (vertex.Pos.y <mMins.y)  mMins.y = vertex.Pos.y;

	if (vertex.Pos.z >mMaxs.z)   mMaxs.z = vertex.Pos.z;
	if (vertex.Pos.z <mMins.z)  mMins.z = vertex.Pos.z;
}

void AABB::ComputeCenterExt()
{
	mCenter = XMFLOAT3((mMaxs.x + mMins.x) * 0.5f, (mMaxs.y + mMins.y) * 0.5f, (mMaxs.z + mMins.z) * 0.5f);
	mExtents = XMFLOAT3(mMaxs.x - mCenter.x, mMaxs.y - mCenter.y, mMaxs.z - mCenter.z);
}

void AABB::ComputeMinMax()
{
	mMins = XMFLOAT3(mCenter.x - mExtents.x, mCenter.y - mExtents.y, mCenter.z - mExtents.z);
	mMaxs = XMFLOAT3(mCenter.x + mExtents.x, mCenter.y + mExtents.y, mCenter.z + mExtents.z);
}

void AABB::GetCorners( std::vector<XMVECTOR>& corners )
{
	//This map enables us to use a for loop and do vector math.
	static const XMVECTORF32 vExtentsMap[] =
	{
		{ 1.0f, 1.0f, -1.0f, 1.0f },    //near-upper-right
		{ -1.0f, 1.0f, -1.0f, 1.0f },   //near-upper-left
		{ 1.0f, -1.0f, -1.0f, 1.0f },   //near-lower-right
		{ -1.0f, -1.0f, -1.0f, 1.0f },  //near-lower-left
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f },
		{ -1.0f, -1.0f, 1.0f, 1.0f }
	};

	XMVECTOR extents = XMLoadFloat3(&mExtents);
	XMVECTOR center = XMLoadFloat3(&mCenter);

	for (INT index = 0; index < 8; ++index)
	{
		corners.push_back(XMVectorMultiplyAdd(vExtentsMap[index], extents, center));
	}
}