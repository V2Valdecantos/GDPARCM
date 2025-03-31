#pragma once

#include "Camera.h"

namespace GDEngine
{
	class OrbitCamera : public Camera
	{
	private:
		AGameObject* objectToOrbit;

		float speed = 0.0f;
		float angle = 0.0f;

	public:
		OrbitCamera(std::string name);
		~OrbitCamera();

	public:
		void update(float deltaTime) override;
		void setObjectToOrbit(AGameObject* objectToOrbit);
		void setSpeed(float speed);
	};
}