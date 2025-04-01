#pragma once

#include "GameObject.h"

#include "Vector2D.h"

namespace GDEngine {
	class Cylinder : public AGameObject
	{
	protected:
		//vertex vertex_list[8];
		//Vector3D color;

		VertexBuffer* m_vertexBuffer;
		ConstantBuffer* m_constantBuffer;
		IndexBuffer* m_indexBuffer;

		float height = 2.0f;
		int slices = 32;
		float radius = 1.0f;
		float stacks = 1.0f;

	public:
		Cylinder(std::string name);
		Cylinder(std::string guid, std::string name);
		~Cylinder() override;

	public:
		void onCreate() override;
		void update(float deltaTime) override;
		void draw(int width, int height) override;
		void onDestroy() override;

	private:
		struct vertex
		{
			Vector3D position;
			Vector2D texCoord;
		};
	};
}