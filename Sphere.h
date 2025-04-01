#pragma once

#include "GameObject.h"

#include "Vector2D.h"

namespace GDEngine {
	class Sphere : public AGameObject
	{
	protected:
		VertexBuffer* m_vertexBuffer;
		ConstantBuffer* m_constantBuffer;
		IndexBuffer* m_indexBuffer;

		int rings = 32;
		int sectors = 32;
		float height = 0.5f;
		float radius = 1.0f;

	public:
		Sphere(std::string name);
		Sphere(std::string guid, std::string name);
		~Sphere() override;

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