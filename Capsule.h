#pragma once

#include "GameObject.h"

#include "Vector2D.h"

namespace GDEngine {
	class Capsule : public AGameObject
	{
	protected:
		//vertex vertex_list[8];
		//Vector3D color;

		VertexBuffer* m_vertexBuffer;
		ConstantBuffer* m_constantBuffer;
		IndexBuffer* m_indexBuffer;

		int stacks = 32;
		int slices = 32;
		float height = 0.5f;
		float radius = 1.0f;

	public:
		Capsule(std::string name);
		Capsule(std::string guid, std::string name);
		~Capsule() override;

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