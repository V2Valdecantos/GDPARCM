#pragma once
#include <string>

#include "GameObject.h"

namespace GDEngine
{
	class MeshRenderer;
	class PhysicsSystem;
	class BaseComponentSystem
	{
	private:
		PhysicsSystem* m_physicsSystem;

	public:
		PhysicsSystem* getPhysicsSystem();
		void createMeshRendererFromFile(std::string guid, std::string name, AGameObject* gameObject,
			AComponent::ComponentType type, std::string filePath);
		void createTextureComponentFromFile(std::string guid, std::string name, AGameObject* gameObject,
			AComponent::ComponentType type, TextureName texName);

	private:
		static BaseComponentSystem* P_SHARED_INSTANCE;

	private:
		BaseComponentSystem();
		~BaseComponentSystem();
		BaseComponentSystem(BaseComponentSystem const&);
		BaseComponentSystem& operator = (BaseComponentSystem const&);

	public:
		static BaseComponentSystem* getInstance();
		static void initialize();
		static void destroy();
	};
}
