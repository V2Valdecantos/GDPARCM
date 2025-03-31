#pragma once

#include "MeshObject.h"

namespace GDEngine
{
	class Teapot : public MeshObject
	{
	public:
		Teapot(std::string name);
		Teapot(std::string guid, std::string name);
		~Teapot() override;

	public:
		void onCreate() override;
		void update(float deltaTime) override;
		void draw(int width, int height) override;
		void onDestroy() override;
	};
}