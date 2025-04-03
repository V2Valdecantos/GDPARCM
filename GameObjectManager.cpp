#include "GameObjectManager.h"

#include "BaseComponentSystem.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"

#include "Armadillo.h"
#include "Bunny.h"
#include "Cube.h"
#include "Capsule.h"
#include "Cylinder.h"
#include "Lucy.h"
#include "Sphere.h"
#include "Plane.h"
#include "Quad.h"
#include "Teapot.h"
#include "TexturedCube.h"

#include "EditorAction.h"
#include "EmptyGameObject.h"

#include "Logger.h"

using namespace GDEngine;

GameObjectManager* GameObjectManager::P_SHARED_INSTANCE = nullptr;

void GameObjectManager::createCube()
{
	Cube* cube = new Cube("Cube");
	this->addObject(cube);
}

void GameObjectManager::createPhysicsCube()
{
	Cube* cube = new Cube("Physics Cube");
	cube->setPosition(0.0f, 20.0f, 0.0f);
	cube->update(0);
	this->addObject(cube);
	cube->attachComponent(new PhysicsComponent("PhysicsComponent " + cube->getName(), cube));
	cube->setPhysics(true);
}

void GameObjectManager::createPhysicsPlane()
{
	Cube* plane = new Cube("Physics Plane");
	plane->setScale(64, 0.5f, 64);
	plane->setPhysics(true);
	this->addObject(plane);
	plane->attachComponent(new PhysicsComponent("PhysicsComponent " + plane->getName(), plane));
	PhysicsComponent* component = (PhysicsComponent*)plane->findComponentOfType(AComponent::ComponentType::Physics, "PhysicsComponent " + plane->getName());
	component->getRigidBody()->setType(BodyType::KINEMATIC);

	/*Plane* plane = new Plane("Physics Plane");
	plane->attachComponent(new PhysicsComponent("PhysicsComponent", plane));
	PhysicsComponent* component = (PhysicsComponent*)plane->findComponentOfType(AComponent::ComponentType::Physics, "PhysicsComponent");
	component->getRigidBody()->setType(BodyType::KINEMATIC);
	this->addObject(plane);*/
}

void GameObjectManager::createTexturedCube()
{
	TexturedCube* cube = new TexturedCube("Textured Cube");
	this->addObject(cube);
}

void GameObjectManager::createTexturedCapsule()
{
	Capsule* capsule = new Capsule("Capsule");
	this->addObject(capsule);
}

void GameObjectManager::createTexturedCylinder()
{
	Cylinder* cyl = new Cylinder("Cylinder");
	this->addObject(cyl);
}

void GameObjectManager::createTexturedSphere()
{
	Sphere* sphere = new Sphere("Sphere");
	this->addObject(sphere);
}


void GameObjectManager::createPlane()
{
	Plane* plane = new Plane("Plane");
	this->addObject(plane);
}

void GameObjectManager::createQuad()
{
	Quad* quad = new Quad("Quad");
	this->addObject(quad);
}

void GameObjectManager::createTeapot()
{
	Teapot* teapot = new Teapot("Teapot");
	this->addObject(teapot);
}

void GameObjectManager::createBunny()
{
	Bunny* bunny = new Bunny("Bunny");
	this->addObject(bunny);
}

void GameObjectManager::createArmadillo()
{
	Armadillo* armadillo = new Armadillo("Armadillo");
	this->addObject(armadillo);
}

void GDEngine::GameObjectManager::createLucy()
{
	Lucy* lucy = new Lucy("Lucy");
	this->addObject(lucy);
}

void GameObjectManager::update(float deltaTime)
{
	for (AGameObject* gameObject : this->m_gameObjectList)
	{
		if (gameObject->isActive())
		{
			gameObject->update(deltaTime);
		}
	}
}

void GameObjectManager::draw(int width, int height)
{
	this->guard.lock();
	for (AGameObject* gameObject : this->m_gameObjectList)
	{
		if (gameObject->isActive() && this->isViewable(gameObject))
			gameObject->draw(width, height);
	}
	this->guard.unlock();
}

GameObjectManager::GameObjectList GameObjectManager::getAllObjects()
{
	return this->m_gameObjectList;
}

AGameObject* GameObjectManager::findObjectByName(std::string name)
{
	if (this->m_gameObjectTable[name] != NULL)
	{
		return this->m_gameObjectTable[name];
	}
	else
	{
		Logger::error(this, "Object " + name + " is not found");
		return NULL;
	}
}

void GameObjectManager::addObject(AGameObject* gameObject)
{
	this->guard.lock();

	if (this->m_gameObjectTable[gameObject->getName()] != NULL)
	{
		int count = 1;
		std::string newName = gameObject->getName() + " " + std::to_string(count);

		while (this->m_gameObjectTable[newName] != NULL)
		{
			count++;
			newName = gameObject->getName() + " " + std::to_string(count);
		}
		gameObject->m_name = newName;
		this->m_gameObjectTable[newName] = gameObject;
	}
	else
	{
		this->m_gameObjectTable[gameObject->getName()] = gameObject;
	}

	this->m_gameObjectList.push_back(gameObject);
	gameObject->onCreate();
	Logger::log(gameObject->getName() + " added to hierarchy");

	this->guard.unlock();
}

void GameObjectManager::deleteObject(AGameObject* gameObject)
{
	int index = -1;

	for (int i = 0; i < this->m_gameObjectList.size() && index == -1; i++)
	{
		if (this->m_gameObjectList[i] == gameObject)
			index = i;
	}

	if (index != -1)
	{
		this->m_gameObjectTable.erase(this->m_gameObjectList[index]->getName());
		this->m_gameObjectList.erase(this->m_gameObjectList.begin() + index);

		this->m_selectedObject->setParent(nullptr);

		for (AGameObject* child : gameObject->getChildren())
		{
			deleteObject(child);  
		}

		if (gameObject->getParent() != nullptr)
		{
			gameObject->getParent()->removeChild(gameObject);  
		}

		gameObject->onDestroy();
		delete gameObject;
	}
}

void GameObjectManager::deleteObjectByName(std::string name)
{
	AGameObject* object = this->findObjectByName(name);

	if (object != NULL)
	{
		this->deleteObject(object);
	}
}

void GameObjectManager::deleteAllObjects()
{
	if (!this->m_gameObjectList.empty())
	{
		for (AGameObject* gameObject : this->m_gameObjectList)
			gameObject->onDestroy();
		this->m_gameObjectList.clear();
		this->m_gameObjectTable.clear();
	}
}

void GameObjectManager::setSelectedObject(GUID guid)
{
	for (AGameObject* object : m_gameObjectList)
	{
		GUID objectGuid = object->getGuid();
		if (IsEqualGUID(objectGuid, guid))
		{
			this->setSelectedObject(object);

			return;
		}
	}
}

void GameObjectManager::setSelectedObject(std::string name)
{
	if (this->m_gameObjectTable[name] != NULL)
	{
		this->setSelectedObject(this->m_gameObjectTable[name]);
	}
}

void GameObjectManager::setSelectedObject(AGameObject* gameObject)
{
	this->m_selectedObject = gameObject;

	if (this->m_selectedObjects.size() == 0)
		this->m_selectedObjects.push_back(gameObject);
	else if (this->m_multiselectMode)
		this->m_selectedObjects.push_back(gameObject);
	else if (!this->m_multiselectMode) {
		this->m_selectedObjects.clear();
		this->m_selectedObjects.push_back(gameObject);
	}
}

AGameObject* GameObjectManager::getSelectedObject()
{
	return this->m_selectedObject;
}

std::vector<AGameObject*> GameObjectManager::getSelectedObjects()
{
	return this->m_selectedObjects;
}

bool GDEngine::GameObjectManager::isSelected(AGameObject* obj)
{
	for (AGameObject* object : m_selectedObjects)
	{
		if (obj == object)
		{
			return true;
		}
	}

	return false;
}

bool GDEngine::GameObjectManager::isViewable(AGameObject* obj)
{
	for (AGameObject* object : m_viewables)
	{
		if (obj == object)
		{
			return true;
		}
	}

	return false;
}

void GameObjectManager::saveEditStates()
{
	for (AGameObject* gameObject : m_gameObjectList) {
		gameObject->saveEditState();
	}
}

void GameObjectManager::restoreEditStates()
{
	for (AGameObject* gameObject : m_gameObjectList) {
		gameObject->restoreEditState();
	}
}

void GameObjectManager::applyAction(EditorAction* action)
{
	//Logger::log(action->getOwnerName());
	AGameObject* gameObject = this->findObjectByName(action->getOwnerName());
	if (gameObject != nullptr)
	{
		//gameObject->setLocalMatrix(action->getStoredMatrix().getMatrix());
		gameObject->setPosition(action->getStoredPosition());
		gameObject->setRotation(action->getStoredOrientation().x, action->getStoredOrientation().y, action->getStoredOrientation().z);
		gameObject->setScale(action->getStoredScale());
	}
}

void GameObjectManager::setPhysics(bool physics)
{
	for (AGameObject* gameObject : m_gameObjectList)
	{
		AGameObject::ComponentList physicsList = gameObject->getComponentsOfType(AComponent::Physics);

		if (physicsList.size() != 0)
			gameObject->setPhysics(physics);
	}
}

bool GameObjectManager::getMultiselectMode()
{
	return this->m_multiselectMode;
}

void GameObjectManager::setMultiselectMode(bool multiselect)
{
	this->m_multiselectMode = multiselect;
}

void GameObjectManager::setViewableObjects(std::vector<AGameObject*> viewables)
{
	this->m_viewables = viewables;
}

AGameObject* GameObjectManager::createObjectFromFile(std::string objectGuid, std::string objectName, std::string classType, Vector3D position, Vector3D rotation,
	Vector3D scale)
{
	if (classType == typeid(Cube).raw_name())
	{
		Cube* cube = new Cube(objectGuid, objectName);
		cube->setPosition(position);
		cube->setRotation(rotation);
		cube->setScale(scale);
		this->addObject(cube);
		return cube;
	}
	if (classType == typeid(Cylinder).raw_name())
	{
		Cylinder* cylinder = new Cylinder(objectGuid, objectName);
		cylinder->setPosition(position);
		cylinder->setRotation(rotation);
		cylinder->setScale(scale);
		this->addObject(cylinder);
		return cylinder;
	}
	if (classType == typeid(Capsule).raw_name())
	{
		Capsule* capsule = new Capsule(objectGuid, objectName);
		capsule->setPosition(position);
		capsule->setRotation(rotation);
		capsule->setScale(scale);
		this->addObject(capsule);
		return capsule;
	}
	if (classType == typeid(Sphere).raw_name())
	{
		Sphere* sphere = new Sphere(objectGuid, objectName);
		sphere->setPosition(position);
		sphere->setRotation(rotation);
		sphere->setScale(scale);
		this->addObject(sphere);
		return sphere;
	}
	if (classType == typeid(Plane).raw_name())
	{
		Plane* plane = new Plane(objectGuid, objectName);
		plane->setPosition(position);
		plane->setRotation(rotation);
		plane->setScale(scale);
		this->addObject(plane);
		return plane;
	}
	if (classType == typeid(Quad).raw_name())
	{
		Quad* quad = new Quad(objectGuid, objectName);
		quad->setPosition(position);
		quad->setRotation(rotation);
		quad->setScale(scale);
		this->addObject(quad);
		return quad;
	}
	if (classType == typeid(Teapot).raw_name())
	{
		Teapot* teapot = new Teapot(objectGuid, objectName);
		teapot->setPosition(position);
		teapot->setRotation(rotation);
		teapot->setScale(scale);
		this->addObject(teapot);
		return teapot;
	}
	if (classType == typeid(Bunny).raw_name())
	{
		Bunny* bunny = new Bunny(objectGuid, objectName);
		bunny->setPosition(position);
		bunny->setRotation(rotation);
		bunny->setScale(scale);
		this->addObject(bunny);
		return bunny;
	}
	if (classType == typeid(Armadillo).raw_name())
	{
		Armadillo* armadillo = new Armadillo(objectGuid, objectName);
		armadillo->setPosition(position);
		armadillo->setRotation(rotation);
		armadillo->setScale(scale);
		this->addObject(armadillo);
		return armadillo;
	}
	return nullptr;
}

AGameObject* GDEngine::GameObjectManager::createObjectFromTextFile(std::string objectName, std::string classType, Vector3D position, Vector3D rotation, Vector3D scale, bool rb, int physType)
{
	if (classType == typeid(Cube).raw_name())
	{
		Cube* cube = new Cube(objectName);
		cube->setPosition(position);
		cube->setRotation(rotation);
		cube->setScale(scale);
		if (rb)
		{
			PhysicsComponent* rb = new PhysicsComponent("PhysicsComponent " + objectName, cube);
			cube->attachComponent(rb);
			rb->setBodyType((BodyType)physType);
		}

		this->addObject(cube);

			
		return cube;
	}
	if (classType == typeid(Cylinder).raw_name())
	{
		Cylinder* cylinder = new Cylinder(objectName);
		cylinder->setPosition(position);
		cylinder->setRotation(rotation);
		cylinder->setScale(scale);
		this->addObject(cylinder);
		return cylinder;
	}
	if (classType == typeid(Capsule).raw_name())
	{
		Capsule* capsule = new Capsule(objectName);
		capsule->setPosition(position);
		capsule->setRotation(rotation);
		capsule->setScale(scale);
		this->addObject(capsule);
		return capsule;
	}
	if (classType == typeid(Sphere).raw_name())
	{
		Sphere* sphere = new Sphere(objectName);
		sphere->setPosition(position);
		sphere->setRotation(rotation);
		sphere->setScale(scale);
		this->addObject(sphere);
		return sphere;
	}
	if (classType == typeid(Plane).raw_name())
	{
		Plane* plane = new Plane(objectName);
		plane->setPosition(position);
		plane->setRotation(rotation);
		plane->setScale(scale);
		this->addObject(plane);
		return plane;
	}
	if (classType == typeid(Quad).raw_name())
	{
		Quad* quad = new Quad(objectName);
		quad->setPosition(position);
		quad->setRotation(rotation);
		quad->setScale(scale);
		this->addObject(quad);
		return quad;
	}
	if (classType == typeid(Teapot).raw_name())
	{
		Teapot* teapot = new Teapot(objectName);
		teapot->setPosition(position);
		teapot->setRotation(rotation);
		teapot->setScale(scale);
		this->addObject(teapot);
		return teapot;
	}
	if (classType == typeid(Bunny).raw_name())
	{
		Bunny* bunny = new Bunny(objectName);
		bunny->setPosition(position);
		bunny->setRotation(rotation);
		bunny->setScale(scale);
		this->addObject(bunny);
		return bunny;
	}
	if (classType == typeid(Armadillo).raw_name())
	{
		Armadillo* armadillo = new Armadillo(objectName);
		armadillo->setPosition(position);
		armadillo->setRotation(rotation);
		armadillo->setScale(scale);
		this->addObject(armadillo);
		return armadillo;
	}
	return nullptr;
}



GameObjectManager::GameObjectManager()
{
	Logger::log(P_SHARED_INSTANCE, "Initialized");

	this->m_multiselectMode = false;
}

GameObjectManager::~GameObjectManager()
{
	this->deleteAllObjects();
	P_SHARED_INSTANCE = nullptr;
	Logger::log(P_SHARED_INSTANCE, "Destroyed");
}

GameObjectManager::GameObjectManager(const GameObjectManager&) {}

GameObjectManager* GameObjectManager::getInstance() {
	return P_SHARED_INSTANCE;
}

void GameObjectManager::initialize()
{
	if (P_SHARED_INSTANCE)
	{
		Logger::throw_exception("Game Object Manager already created");
	}
	P_SHARED_INSTANCE = new GameObjectManager();
	
}

void GameObjectManager::destroy()
{
	delete P_SHARED_INSTANCE;
}
