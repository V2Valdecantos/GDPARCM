#pragma once

#include <string>
#include <ostream>

#include "Vector2D.h"
#include "Vector3D.h"

namespace GDEngine
{
	class VertexMesh
	{
	public:
		Vector3D position;
		Vector2D texCoord;

	public:
		VertexMesh() : position(), texCoord() {}
		VertexMesh(Vector3D position, Vector2D texCoord) : position(position), texCoord(texCoord) {}
		VertexMesh(const VertexMesh& vertex) : position(vertex.position), texCoord(vertex.texCoord) {}
		~VertexMesh() {}

	public:
	};
}