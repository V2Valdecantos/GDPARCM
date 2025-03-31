#pragma once

#include "GameObject.h"

namespace GDEngine {
	class Border : public AGameObject
	{
	private:
		VertexBuffer* m_vertexBuffer;
		ConstantBuffer* m_constantBuffer;

	public:
		Border(std::string name);
		~Border();

	public:
		void onCreate() override;
		void update(float deltaTime) override;
		void draw(int width, int height) override;
		void onDestroy() override;
	};
}