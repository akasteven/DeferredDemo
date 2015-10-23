#include "GeoGenerator.h"


GeoGenerator::GeoGenerator()
{
}


GeoGenerator::~GeoGenerator()
{
}


void GeoGenerator::GenCuboid(float w, float h, float d, Mesh & mesh)
{
	float hw = w / 2.0f;
	float hh = h / 2.0f;
	float hd = d / 2.0f;

	Vertex::VertexPNT v[24] = {

		//front
		Vertex::VertexPNT( -hw, -hh, -hd, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f ),
		Vertex::VertexPNT( hw, -hh, -hd, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
		Vertex::VertexPNT( hw, hh, -hd, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f ),
		Vertex::VertexPNT( -hw, hh, -hd, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),

		//back
		Vertex::VertexPNT(-hw, -hh, hd, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		Vertex::VertexPNT( hw, -hh, hd, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex::VertexPNT( hw, hh, hd, 0.0f, 0.0f, 1.0f, 0.0f , 0.0f),
		Vertex::VertexPNT( -hw, hh, hd, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),

		//top
		Vertex::VertexPNT(-hw, hh, -hd, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex::VertexPNT(hw, hh, -hd, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),
		Vertex::VertexPNT(hw, hh, hd, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
		Vertex::VertexPNT(-hw, hh, hd, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),

		//bottom
		Vertex::VertexPNT(-hw, -hh, -hd, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f),
		Vertex::VertexPNT(hw, -hh, -hd, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),
		Vertex::VertexPNT(hw, -hh, hd, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f),
		Vertex::VertexPNT(-hw, -hh, hd, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),

		//left
		Vertex::VertexPNT(-hw, -hh, hd, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		Vertex::VertexPNT(-hw, -hh, -hd, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
		Vertex::VertexPNT(-hw, hh, -hd, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		Vertex::VertexPNT(-hw, hh, hd, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f),

		//right
		Vertex::VertexPNT(hw, -hh, hd, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f),
		Vertex::VertexPNT(hw, -hh, -hd, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f),
		Vertex::VertexPNT(hw, hh, -hd, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
		Vertex::VertexPNT(hw, hh, hd, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f),
	};
	mesh.vertices.assign(v, v + 24);

	DWORD i[36] = 
	{
		0, 1, 2,
		0, 2, 3,
		5, 4, 7,
		5, 7, 6,
		8, 9, 10,
		8, 10, 11,
		13, 12, 15,
		13, 15, 14,
		16, 17, 18,
		16, 18, 19,
		21, 20, 23,
		21, 23, 22
	};

	mesh.indices.assign( i , i + 36);
}

void GeoGenerator::GenSphere(float radius, UINT sliceCnt, UINT stackCnt, Mesh & mesh)
{
	mesh.vertices.clear();
	mesh.indices.clear();

	Vertex::VertexPNT topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, 0.0f);
	Vertex::VertexPNT  bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

	mesh.vertices.push_back(topVertex);

	float phiStep = XM_PI / stackCnt;
	float thetaStep = 2.0f*XM_PI / sliceCnt;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stackCnt - 1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCnt; ++j)
		{
			float theta = j*thetaStep;

			Vertex::VertexPNT v;

			// spherical to cartesian
			v.Pos.x = radius*sinf(phi)*cosf(theta);
			v.Pos.y = radius*cosf(phi);
			v.Pos.z = radius*sinf(phi)*sinf(theta);


			XMVECTOR p = XMLoadFloat3(&v.Pos);
			XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

			v.Tex.x = theta / XM_2PI;
			v.Tex.y = phi / XM_PI;

			mesh.vertices.push_back(v);
		}
	}

	mesh.vertices.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= sliceCnt; ++i)
	{
		mesh.indices.push_back(0);
		mesh.indices.push_back(i + 1);
		mesh.indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCnt + 1;
	for (UINT i = 0; i < stackCnt - 2; ++i)
	{
		for (UINT j = 0; j < sliceCnt; ++j)
		{
			mesh.indices.push_back(baseIndex + i*ringVertexCount + j);
			mesh.indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			mesh.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			mesh.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			mesh.indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			mesh.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)mesh.vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCnt; ++i)
	{
		mesh.indices.push_back(southPoleIndex);
		mesh.indices.push_back(baseIndex + i);
		mesh.indices.push_back(baseIndex + i + 1);
	}
}
