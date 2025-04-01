#include "PhysicsSystem.h"

#include "EngineTime.h"
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "Logger.h"

using namespace GDEngine;

PhysicsSystem::PhysicsSystem()
{
	this->m_physicsCommon = new PhysicsCommon();

	PhysicsWorld::WorldSettings settings;
	settings.defaultVelocitySolverNbIterations = 50;
	settings.gravity = Vector3(0, -9.81, 0);

	this->m_physicsWorld = this->m_physicsCommon->createPhysicsWorld(settings);

	Logger::log(this, "Created Physics World");
	Logger::log(this, "Initialize");
}

PhysicsSystem::~PhysicsSystem()
{
	delete this->m_physicsCommon;
}

void PhysicsSystem::createComponentFromFile(std::string guid, std::string name, AGameObject* gameObject,
	AComponent::ComponentType type, float mass,bool gravity, BodyType bodyType, float linearDrag, float angularDrag, uint8_t constraints)
{
	PhysicsComponent* newComponent = new PhysicsComponent(guid, name, gameObject);
	newComponent->setMass(mass);
	newComponent->setUseGravity(gravity);
	newComponent->setBodyType(bodyType);
	newComponent->setLinearDrag(linearDrag);
	newComponent->setAngularDrag(angularDrag);
	newComponent->setConstraints(constraints);
	gameObject->attachComponent(newComponent);
}

void PhysicsSystem::registerComponent(PhysicsComponent* component)
{
	this->m_componentMap[component->getName()] = component;
	this->m_componentList.push_back(component);
	Logger::log(component->getName() + " registered Component");
}

void PhysicsSystem::unregisterComponent(PhysicsComponent* component)
{
	if (this->m_componentMap[component->getName()] == NULL)
	{
		Logger::log(component->getName() + " not registered in Physics System");
		return;
	}

	int index = -1;

	this->m_componentMap.erase(component->getName());
	for (int i = 0; i < this->m_componentList.size() && index == -1; i++)
	{
		if (this->m_componentList[i] == component)
			index = i;
	}

	if (index != -1)
	{
		this->m_componentList.erase(this->m_componentList.begin() + index);
	}

	Logger::log("Unregistered " + component->getName());
}

void PhysicsSystem::unregisterComponentByName(std::string name)
{
	if (this->m_componentMap[name] != NULL)
	{
		this->unregisterComponent(this->m_componentMap[name]);
	}
}

PhysicsComponent* PhysicsSystem::findComponentByName(std::string name)
{
	if (this->m_componentMap[name] == NULL)
		Logger::error(this, "Component " + name + " is not found");

	return this->m_componentMap[name];
}

PhysicsSystem::PhysicsComponentList PhysicsSystem::getAllComponents()
{
	return this->m_componentList;
}

void PhysicsSystem::updateAllComponents()
{
	if (EngineTime::getDeltaTime() > 0.0f)
	{
		this->m_physicsWorld->update(EngineTime::getDeltaTime());
		for (int i = 0; i < this->m_componentList.size(); i++)
		{
			if (m_componentList[i]->getOwner()->isActive())
				this->m_componentList[i]->perform(EngineTime::getDeltaTime());
		}
	}
}

PhysicsWorld* PhysicsSystem::getPhysicsWorld()
{
	return this->m_physicsWorld;
}

PhysicsCommon* PhysicsSystem::getPhysicsCommon()
{
	return this->m_physicsCommon;
}