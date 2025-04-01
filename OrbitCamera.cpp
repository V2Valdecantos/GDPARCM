#include "OrbitCamera.h"

using namespace GDEngine;

OrbitCamera::OrbitCamera(std::string name) : Camera(name)
{
}

OrbitCamera::~OrbitCamera()
{
}

void OrbitCamera::update(float deltaTime)
{
	Camera::update(deltaTime);
	angle += deltaTime * speed;

	if (angle > 360.0f)
		angle = 0.0f;

	float radian = angle * ((atan(1.0f) * 4.0f) / 180.0f);

	float radius = 2.0f;

	float x = objectToOrbit->getLocalPosition().x + (cos(radian) * radius);
	float z = objectToOrbit->getLocalPosition().z + (sin(radian) * radius);

	this->m_localRotation.y = -radian - (90.0f * ((atan(1.0f) * 4.0f) / 180.0f));

	this->setPosition(Vector3D(x, 0, z));
}

void OrbitCamera::setObjectToOrbit(AGameObject* objectToOrbit)
{
	this->objectToOrbit = objectToOrbit;
}

void OrbitCamera::setSpeed(float speed)
{
	this->speed = speed;
}
