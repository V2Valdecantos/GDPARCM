#pragma once

#include "GameObject.h"

namespace GDEngine {
	class Cube : public AGameObject
	{
	protected:
		//vertex vertex_list[8];
		//Vector3D color;

		VertexBuffer* m_vertexBuffer;
		ConstantBuffer* m_constantBuffer;
		IndexBuffer* m_indexBuffer;

		float deltaRotation;
		float angle;
		float speed = 1.0f;
		bool startRotate = false;
		float elapsedTime = 0.0f;
		float deltaScale = 0.0f;


	public:
		Cube(std::string name);
		Cube(std::string guid, std::string name);
		~Cube() override;

	public:
		void onCreate() override;
		void update(float deltaTime) override;
		void draw(int width, int height) override;
		void onDestroy() override;

		void setSpeed(float speed);
		void toggleTexture() override;
	};
}