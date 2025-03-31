#include "PhysicsComponent.h"

#include "BaseComponentSystem.h"
#include "PhysicsSystem.h"
#include "GameObject.h"
#include "Vector3D.h"

using namespace GDEngine;

PhysicsComponent::PhysicsComponent(std::string name, AGameObject* owner) : AComponent(name, Physics, owner)
{
	BaseComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(this);
	PhysicsCommon* physicsCommon = BaseComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsCommon();
	PhysicsWorld* physicsWorld = BaseComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();

	const Vector3D scale = this->getOwner()->getLocalScale();
	const Vector3D position = this->getOwner()->getLocalPosition();
	const Vector3D rotation = this->getOwner()->getLocalRotation();

	Quaternion quaternion = Quaternion::fromEulerAngles(rotation.x, rotation.y, rotation.z);
	//Transform transform = Transform(Vector3(position.x, position.y, position.z), quaternion);

	Transform transform;
	transform.setFromOpenGL(this->getOwner()->getPhysicsLocalMatrix());

	BoxShape* boxShape = physicsCommon->createBoxShape(Vector3(scale.x / 1.0f, scale.y / 1.0f, scale.z / 1.0f));

	this->m_rigidBody = physicsWorld->createRigidBody(transform);
	this->m_rigidBody->addCollider(boxShape, transform);
	this->m_rigidBody->updateMassPropertiesFromColliders();
	this->m_rigidBody->setMass(this->m_mass);
	this->m_rigidBody->setType(BodyType::DYNAMIC);

	transform = this->m_rigidBody->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);

	//this->getOwner()->setLocalMatrix(matrix);
	//const Vector3 position1 = transform.getPosition();
	//const Quaternion orientation = transform.getOrientation();
	//const Vector3 rotation1 = orientation.getVectorV();

	//this->getOwner()->setPosition(Vector3D(position1.x, position1.y, position1.z));
	//this->getOwner()->setRotation(Vector3D(rotation1.x, rotation1.y, rotation1.z));
}

PhysicsComponent::PhysicsComponent(std::string guid, std::string name, AGameObject* owner) : AComponent(guid, name, Physics, owner)
{
	BaseComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(this);
	PhysicsCommon* physicsCommon = BaseComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsCommon();
	PhysicsWorld* physicsWorld = BaseComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();

	const Vector3D scale = this->getOwner()->getLocalScale();
	const Vector3D position = this->getOwner()->getLocalPosition();
	const Vector3D rotation = this->getOwner()->getLocalRotation();

	Quaternion quaternion = Quaternion::fromEulerAngles(rotation.x, rotation.y, rotation.z);

	Transform transform;
	transform.setFromOpenGL(this->getOwner()->getPhysicsLocalMatrix());

	BoxShape* boxShape = physicsCommon->createBoxShape(Vector3(scale.x / 1.0f, scale.y / 1.0f, scale.z / 1.0f));

	this->m_rigidBody = physicsWorld->createRigidBody(transform);
	this->m_rigidBody->addCollider(boxShape, transform);
	this->m_rigidBody->updateMassPropertiesFromColliders();
	this->m_rigidBody->setMass(this->m_mass);
	this->m_rigidBody->setType(BodyType::DYNAMIC);

	transform = this->m_rigidBody->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);

	this->getOwner()->setLocalMatrix(matrix);
}

PhysicsComponent::~PhysicsComponent()
{
	BaseComponentSystem::getInstance()->getPhysicsSystem()->unregisterComponent(this);
	AComponent::~AComponent();

	PhysicsWorld* physicsWorld = BaseComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();
	physicsWorld->destroyRigidBody(this->m_rigidBody);
}

void PhysicsComponent::perform(float deltaTime)
{
	/*const Transform transform = this->m_rigidBody->getTransform();
	const Vector3 position = transform.getPosition();
	const Quaternion orientation = transform.getOrientation();
	const Vector3 rotation = orientation.getVectorV();

	this->getOwner()->setPosition(Vector3D(position.x, position.y, position.z));
	//this->getOwner()->setRotation(Vector3D(rotation.x, rotation.y, rotation.z));*/
	Transform transform = this->m_rigidBody->getTransform();
	const Vector3 position = transform.getPosition();
	const Quaternion orientation = transform.getOrientation();

	//this->getOwner()->setPosition(Vector3D(position.x, position.y, position.z));
	//this->getOwner()->setOrientation(Vector4D(orientation.x, orientation.y, orientation.z, orientation.w));

	//this->m_rigidBody->setTransform(Transform(position, orientation));
	//transform = Transform(position, quaternion);
	//this->m_rigidBody->setTransform(transform);
	float matrix[16];
	transform.getOpenGLMatrix(matrix);
	//this->getOwner()->setLocalMatrix(Vector4D(orientation.x, orientation.y, orientation.z, orientation.w), matrix);
	this->getOwner()->setLocalMatrix(matrix);
	const Vector3D position2 = this->getOwner()->getLocalPosition();

	
	//this->getOwner()->setLocalMatrix(Vector3D(position.x, position.y, position.z), Vector4D(orientation.x, orientation.y, orientation.z, orientation.w), matrix);
	//Logger::log("Updating Component : " + this->m_name);
}

RigidBody* PhysicsComponent::getRigidBody()
{
	return this->m_rigidBody;
}

float PhysicsComponent::getMass()
{
	return m_mass;
}

bool PhysicsComponent::getUseGravity()
{
	return this->m_rigidBody->isGravityEnabled();
}

BodyType PhysicsComponent::getBodyType()
{
	return this->m_rigidBody->getType();
}

float PhysicsComponent::getLinearDrag()
{
	return this->m_rigidBody->getLinearDamping();
}

float PhysicsComponent::getAngularDrag()
{
	return this->m_rigidBody->getAngularDamping();
}

bool PhysicsComponent::getConstraint(EConstraints constraint)
{
	return (m_constraints & static_cast<uint8_t>(constraint)) == static_cast<uint8_t>(constraint);
}

uint8_t PhysicsComponent::getConstraints()
{
	return m_constraints;
}

void PhysicsComponent::setTransformFromOpenGL(float* matrix)
{
	Transform transform;
	transform.setFromOpenGL(matrix);
	this->m_rigidBody->setTransform(transform);
}

void PhysicsComponent::setMass(float mass)
{
	this->m_mass = mass;
	this->m_rigidBody->setMass(mass);
}

void PhysicsComponent::setUseGravity(const bool isUsingGravity)
{
	this->m_rigidBody->enableGravity(isUsingGravity);
}

void PhysicsComponent::setBodyType(const BodyType type) const
{
	this->m_rigidBody->setType(type);
}

void PhysicsComponent::setLinearDrag(float linearDrag)
{
	if (linearDrag < 0)
		linearDrag = 0;

	this->m_rigidBody->setLinearDamping(linearDrag);
}

void PhysicsComponent::setAngularDrag(float angularDrag)
{
	if (angularDrag < 0)
		angularDrag = 0;

	this->m_rigidBody->setAngularDamping(angularDrag);
}

void PhysicsComponent::setConstraints(EConstraints constraints)
{
	this->m_constraints = static_cast<uint8_t>(constraints);

	// getConstraint returns 1 if true. AxisFactor of ReactPhysics3D is 0 to freeze.
	const Vector3 freezePosition = Vector3(
		!getConstraint(EConstraints::FreezePositionX), 
		!getConstraint(EConstraints::FreezePositionY),
		!getConstraint(EConstraints::FreezePositionZ));

	const Vector3 freezeRotation = Vector3(
		!getConstraint(EConstraints::FreezeRotationX),
		!getConstraint(EConstraints::FreezeRotationY),
		!getConstraint(EConstraints::FreezeRotationZ));

	this->m_rigidBody->setLinearLockAxisFactor(freezePosition);
	this->m_rigidBody->setAngularLockAxisFactor(freezeRotation);
}

void PhysicsComponent::setConstraints(uint8_t constraints)
{
	this->m_constraints = constraints;

	// getConstraint returns 1 if true. AxisFactor of ReactPhysics3D is 0 to freeze.
	const Vector3 freezePosition = Vector3(
		!getConstraint(EConstraints::FreezePositionX),
		!getConstraint(EConstraints::FreezePositionY),
		!getConstraint(EConstraints::FreezePositionZ));

	const Vector3 freezeRotation = Vector3(
		!getConstraint(EConstraints::FreezeRotationX),
		!getConstraint(EConstraints::FreezeRotationY),
		!getConstraint(EConstraints::FreezeRotationZ));

	this->m_rigidBody->setLinearLockAxisFactor(freezePosition);
	this->m_rigidBody->setAngularLockAxisFactor(freezeRotation);
}