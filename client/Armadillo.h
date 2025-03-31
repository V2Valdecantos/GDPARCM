#pragma once

#include "MeshObject.h"

namespace GDEngine {
	class Armadillo : public MeshObject
	{
	public:
		Armadillo(std::string name);
		Armadillo(std::string guid, std::string name);
		~Armadillo() override;

	public:
		void onCreate() override;
		void update(float deltaTime) override;
		void draw(int width, int height) override;
		void onDestroy() override;
	};
}