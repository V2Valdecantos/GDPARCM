#pragma once

#include "GameObject.h"

namespace GDEngine
{
	class Plane : public AGameObject
	{
	private:
		VertexBuffer* m_vertexBuffer;
		IndexBuffer* m_indexBuffer;
		ConstantBuffer* m_constantBuffer;

		float deltaPosition;
		float deltaScale;
		float angle;

	public:
		Plane(std::string name);
		Plane(std::string guid, std::string name);
		~Plane();

	public:
		void onCreate() override;
		void update(float deltaTime) override;
		void draw(int width, int height) override;
		void onDestroy() override;

	protected:
		struct vertex
		{
			Vector3D position;
			Vector3D color;
			Vector3D color2;
		};
	};
}