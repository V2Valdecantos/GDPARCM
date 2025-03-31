#pragma once

#include "Cube.h"

#include "Vector2D.h"

namespace GDEngine
{
	class TexturedCube : public Cube
	{
	public:
		TexturedCube(std::string name);
		~TexturedCube() override;

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