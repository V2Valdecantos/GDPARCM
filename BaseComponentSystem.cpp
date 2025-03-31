#include "BaseComponentSystem.h"

#include "PhysicsSystem.h"
#include "Logger.h"
#include "MeshRenderer.h"
#include "TextureComponent.h"

using namespace GDEngine;

BaseComponentSystem* BaseComponentSystem::P_SHARED_INSTANCE = NULL;

PhysicsSystem* BaseComponentSystem::getPhysicsSystem()
{
	return this->m_physicsSystem;
}

void BaseComponentSystem::createMeshRendererFromFile(std::string guid, std::string name, AGameObject* gameObject,
	AComponent::ComponentType type, std::string filePath)
{
	MeshRenderer* newRenderer = new MeshRenderer(guid, name, gameObject);
	std::wstring wide = std::wstring(filePath.begin(), filePath.end());
	Logger::log(L"Creating Mesh Component From : " + wide);
	gameObject->attachComponent(newRenderer);
	newRenderer->setMeshFromFile(wide.c_str());
}

void BaseComponentSystem::createTextureComponentFromFile(std::string guid, std::string name, AGameObject* gameObject,
	AComponent::ComponentType type, TextureName texName)
{
	TextureComponent* newTexture = new TextureComponent(guid, name, gameObject);

	gameObject->attachComponent(newTexture);
	newTexture->setTexture(texName);
}

BaseComponentSystem::BaseComponentSystem()
{
	try
	{
		this->m_physicsSystem = new PhysicsSystem();
	}
	catch (...)
	{
		Logger::throw_exception("PhysicsSystem not created successfully");
	}
	
	Logger::log(P_SHARED_INSTANCE, "Initialized");
}

BaseComponentSystem::~BaseComponentSystem()
{
	delete this->m_physicsSystem;
	P_SHARED_INSTANCE = nullptr;
	Logger::log(P_SHARED_INSTANCE, "Released");
}

BaseComponentSystem::BaseComponentSystem(const BaseComponentSystem&) {}

BaseComponentSystem* BaseComponentSystem::getInstance()
{
	return P_SHARED_INSTANCE;
}

void BaseComponentSystem::initialize()
{
	if (P_SHARED_INSTANCE)
		Logger::throw_exception("BaseComponentSystem already created");
	P_SHARED_INSTANCE = new BaseComponentSystem();

}

void BaseComponentSystem::destroy()
{
	if (P_SHARED_INSTANCE != NULL)
	{
		delete P_SHARED_INSTANCE;
	}
}
