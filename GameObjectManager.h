#pragma once

#include <unordered_map>
#include <vector>
#include <mutex>

#include "GameObject.h"

namespace GDEngine
{
	class GameObjectManager
	{
	public:
		typedef std::unordered_map<std::string, AGameObject*> GameObjectTable;
		typedef std::vector<AGameObject*> GameObjectList;

	private:
		GameObjectTable m_gameObjectTable;
		GameObjectList m_gameObjectList;
		AGameObject* m_selectedObject;

		std::vector<AGameObject*> m_selectedObjects;
		std::vector<AGameObject*> m_viewables;

		bool m_multiselectMode;
		std::mutex guard;

	public:
		void createCube();
		void createPhysicsCube();
		void createPhysicsPlane();
		void createTexturedCube();
		void createTexturedCapsule();
		void createTexturedCylinder();
		void createTexturedSphere();
		void createPlane();
		void createQuad();
		void createTeapot();
		void createBunny();
		void createArmadillo();
		void createLucy();

		void update(float deltaTime);
		void draw(int width, int height);

		GameObjectList getAllObjects();
		AGameObject* findObjectByName(std::string name);
		void addObject(AGameObject* gameObject);
		void deleteObject(AGameObject* gameObject);
		void deleteObjectByName(std::string name);
		void deleteAllObjects();
		void setSelectedObject(std::string name);
		void setSelectedObject(GUID guid);
		void setSelectedObject(AGameObject* gameObject);
		AGameObject* getSelectedObject();
		std::vector<AGameObject*> getSelectedObjects();
		bool isSelected(AGameObject* obj);
		bool isViewable(AGameObject* obj);

		void saveEditStates();
		void restoreEditStates();
		void applyAction(EditorAction* action);

		void setPhysics(bool physics);
		bool getMultiselectMode();
		void setMultiselectMode(bool multiselect);

		void setViewableObjects(std::vector<AGameObject*> viewables);

		AGameObject* createObjectFromFile(std::string objectGuid, std::string objectName, std::string classType, Vector3D position, Vector3D rotation, Vector3D scale);
		AGameObject* createObjectFromTextFile(std::string objectName, std::string classType, Vector3D position, Vector3D rotation, Vector3D scale, bool rb, int physType);

	private:
		static GameObjectManager* P_SHARED_INSTANCE;

	private:
		GameObjectManager();
		~GameObjectManager();
		GameObjectManager(const GameObjectManager&);
		GameObjectManager& operator = (const GameObjectManager&);

	public:
		static GameObjectManager* getInstance();
		static void initialize();
		static void destroy();
	};
}