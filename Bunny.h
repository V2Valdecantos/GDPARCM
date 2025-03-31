#pragma once

#include "MeshObject.h"

namespace GDEngine {
	class Bunny : public MeshObject
	{
	public:
		Bunny(std::string name);
		Bunny(std::string guid, std::string name);
		~Bunny() override;

	public:
		void onCreate() override;
		void update(float deltaTime) override;
		void draw(int width, int height) override;
		void onDestroy() override;
	};
}