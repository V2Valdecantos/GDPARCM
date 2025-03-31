#pragma once

#include "AComponent.h"
#include <reactphysics3d/reactphysics3d.h>

namespace GDEngine
{
	using namespace reactphysics3d;
	class PhysicsComponent : public AComponent
	{
	public:
		enum class EConstraints : uint8_t
		{
			None = 0,
			FreezePositionX = 1 << 0,
			FreezePositionY = 1 << 1,
			FreezePositionZ = 1 << 2,
			FreezeRotationX = 1 << 3,
			FreezeRotationY = 1 << 4,
			FreezeRotationZ = 1 << 5,
			FreezePosition = FreezePositionX | FreezePositionY | FreezePositionZ,
			FreezeRotation = FreezeRotationX | FreezeRotationY | FreezeRotationZ,
			FreezeAll = FreezePosition | FreezeRotation
		};

	private:
		RigidBody* m_rigidBody;
		float m_mass = 1000.0f;
		uint8_t m_constraints = 0;

	public:
		PhysicsComponent(std::string name, AGameObject* owner);
		PhysicsComponent(std::string guid, std::string name, AGameObject* owner);
		~PhysicsComponent() override;

	public:
		void perform(float deltaTime) override;
		RigidBody* getRigidBody();

		float getMass();
		bool getUseGravity();
		BodyType getBodyType();
		float getLinearDrag();
		float getAngularDrag();
		bool getConstraint(EConstraints constraint);
		uint8_t getConstraints();

		void setTransformFromOpenGL(float* matrix);
		void setMass(float mass);
		void setUseGravity(bool isUsingGravity);
		void setBodyType(BodyType type) const;
		void setLinearDrag(float linearDrag);
		void setAngularDrag(float angularDrag);
		void setConstraints(EConstraints constraints);
		void setConstraints(uint8_t constraints);
	};
}