#include "BoundingSphere.h"
#include "AABB.h"

BoundingSphere::~BoundingSphere()
{
}

void BoundingSphere::BuildFromVertices(Vertex::VertexBase * vertex, int numVer)
{
	Clear();
	if (numVer <= 0)
		return;

}

void BoundingSphere::AddVertex(const Vertex::VertexPNTTan &vertex)
{
	return;
}

void BoundingSphere::BuildFromAABB(AABB * aabb)
{
}

