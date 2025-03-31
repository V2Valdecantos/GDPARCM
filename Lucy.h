#pragma once
#include "MeshObject.h"
namespace GDEngine {
	class Lucy : public MeshObject
	{
	public:
		Lucy(std::string name);
		Lucy(std::string guid, std::string name);
		~Lucy() override;

	public:
		void onCreate() override;
		void update(float deltaTime) override;
		void draw(int width, int height) override;
		void onDestroy() override;
	};
}

