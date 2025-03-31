#pragma once

#include "GameObject.h"

namespace GDEngine {
	class Quad : public AGameObject
	{
	private:
		VertexBuffer* m_vertexBuffer;
		ConstantBuffer* m_constantBuffer;

		float deltaPosition;
		float deltaScale;
		float angle;

	public:
		Quad(std::string name);
		Quad(std::string guid, std::string name);
		~Quad();

	public:
		void onCreate() override;
		void update(float deltaTime) override;
		void draw(int width, int height) override;
		void onDestroy() override;
	};
}