#include "SceneReader.h"

#include <fstream>
#include <vector>

#include "BaseComponentSystem.h"
#include "GameObjectManager.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "StringUtility.h"
#include "TextureComponent.h"
#include "Vector3D.h"
#include "MeshRenderer.h"
#include "json/json.h"

namespace GDEngine
{
	SceneReader::SceneReader(std::string directory)
	{
		m_directory = directory;
	}

	SceneReader::~SceneReader()
	{
	}

	void SceneReader::readFromFile()
	{
		std::string fileDirectory = m_directory + ".txt";
		if (m_directory.find(".txt") != std::string::npos)
		{
			fileDirectory = m_directory;
		}

		std::fstream sceneFile;
		sceneFile.open(fileDirectory, std::ios::in);

		int index = 0;
		std::string readLine;

		
		std::string objectName;
		std::string objectType;
		Vector3D position;
		Vector3D rotation;
		Vector3D scale;
		bool rb = false;
		int physType = 0;
		while (std::getline(sceneFile, readLine))
		{
			if (index == 0)
			{
				if (readLine == "{")
					index++;
			}
			else if (index == 1)
			{
				index++; //skip "GameObject"
			}
			else if (index == 2)
			{
				objectName = readLine;
				index++;
			}
			else if (index == 3)
			{
				objectType = readLine;
				index++;
			}
			else if (index == 4) 
			{
				index++; //skip isActive
			}
			else if (index == 5) {
				std::vector<std::string> stringSplit = StringUtility::split(readLine, ',');
				position = Vector3D(std::stof(stringSplit[0]), std::stof(stringSplit[1]), std::stof(stringSplit[2]));
				index++;
			}
			else if (index == 6) {
				std::vector<std::string> stringSplit = StringUtility::split(readLine, ',');
				scale = Vector3D(std::stof(stringSplit[0]), std::stof(stringSplit[1]), std::stof(stringSplit[2]));
				index++;
			}
			else if (index == 7) {
				std::vector<std::string> stringSplit = StringUtility::split(readLine, ',');
				rotation = Vector3D(std::stof(stringSplit[0]), std::stof(stringSplit[1]), std::stof(stringSplit[2]));
				index++;
			}
			else if (index == 8)
			{
				if (readLine == "PhysicsComponent " + objectName)
					rb = true;

				index++;
				
			}
			else if (index == 9) 
			{
				if (rb = true) 
				{
					physType = std::stoi(readLine);
				}
				index++;
			}
			else if (index == 10) 
			{
				if (readLine == "}")
					index = 0;

				GameObjectManager::getInstance()->createObjectFromTextFile(objectName, objectType, position, rotation, scale, rb, physType);
			}
		}
	}

	void SceneReader::readFromJson()
	{
		std::string fileDirectory = m_directory + ".level";
		if (m_directory.find(".level") != std::string::npos)
		{
			fileDirectory = m_directory;
		}

		std::ifstream sceneFile(fileDirectory, std::ifstream::binary);
		Json::Value scene;

		sceneFile >> scene;

		std::vector<std::string> guidList;

		for (std::string id : scene.getMemberNames()) {
			guidList.push_back(id);
		}

		for (std::string guid : guidList)
		{
			std::string name = scene[guid]["name"].asString();
			std::string type = scene[guid]["type"].asString();

			Vector3D position;
			position.x = scene[guid]["position"]["x"].asFloat();
			position.y = scene[guid]["position"]["y"].asFloat();
			position.z = scene[guid]["position"]["z"].asFloat();

			Vector3D rotation;
			rotation.x = scene[guid]["rotation"]["x"].asFloat();
			rotation.y = scene[guid]["rotation"]["y"].asFloat();
			rotation.z = scene[guid]["rotation"]["z"].asFloat();

			Vector3D scale;
			scale.x = scene[guid]["scale"]["x"].asFloat();
			scale.y = scene[guid]["scale"]["y"].asFloat();
			scale.z = scene[guid]["scale"]["z"].asFloat();

			AGameObject* gameObject = GameObjectManager::getInstance()->createObjectFromFile(guid, name, type, position, rotation, scale);

			std::vector<std::string> componentGuidList;
			for (std::string componentId : scene[guid]["components"].getMemberNames())
			{
				componentGuidList.push_back(componentId);
			}

			for (std::string componentGuid : componentGuidList)
			{
				std::string componentName = scene[guid]["components"][componentGuid]["name"].asString();
				std::string componentClassType = scene[guid]["components"][componentGuid]["class"].asString();
				AComponent::ComponentType componentType = static_cast<AComponent::ComponentType>(scene[guid]["components"][componentGuid]["type"].asInt());

				if (componentClassType == typeid(PhysicsComponent).raw_name())
				{
					float mass = scene[guid]["components"][componentGuid]["mass"].asFloat();
					bool gravity = scene[guid]["components"][componentGuid]["gravity"].asBool();
					BodyType componentBodyType = static_cast<BodyType>(scene[guid]["components"][componentGuid]["body_type"].asInt());
					float linearDrag = scene[guid]["components"][componentGuid]["linear_drag"].asFloat();
					float angularDrag = scene[guid]["components"][componentGuid]["angular_drag"].asFloat();
					uint8_t constraints = scene[guid]["components"][componentGuid]["constraints"].asUInt();

					BaseComponentSystem::getInstance()->getPhysicsSystem()->createComponentFromFile(componentGuid, componentName, gameObject, componentType,
						mass, gravity, componentBodyType, linearDrag, angularDrag, constraints);
				}
				if (componentClassType == typeid(TextureComponent).raw_name())
				{
					int texName = scene[guid]["components"][componentGuid]["texture_name"].asInt();

					BaseComponentSystem::getInstance()->createTextureComponentFromFile(componentGuid, componentName, gameObject, componentType, static_cast<TextureName>(texName));
				}
				if (componentClassType == typeid(MeshRenderer).raw_name())
				{
					std::string filePath = scene[guid]["components"][componentGuid]["file_path"].asString();

					//BaseComponentSystem::getInstance()->createMeshRendererFromFile(componentGuid, componentName, gameObject, componentType,
					//	filePath);
				}
			}

		}
		
	}
}