#include <stddef.h>
#include "GameObjectManager.h"
#include "Food.h"
#include <iostream>

GameObjectManager* GameObjectManager::sharedInstance = NULL;

GameObjectManager* GameObjectManager::getInstance() {
	if (sharedInstance == NULL) {
		//initialize
		sharedInstance = new GameObjectManager();
	}

	return sharedInstance;
}

AGameObject* GameObjectManager::findObjectByName(AGameObject::String name)
{
	if (this->gameObjectMap[name] != NULL) {
		return this->gameObjectMap[name];
	}
	else {
		std::cout << "Object " << name << " not found!";
		return NULL;
	}
}

List GameObjectManager::getAllObjects()
{
	return this->gameObjectList;
}

int GameObjectManager::activeObjects()
{
	return this->gameObjectList.size();
}

void GameObjectManager::processInput(sf::Event event) {
	for (int i = 0; i < this->gameObjectList.size(); i++) {
		if (this->gameObjectList[i]->getIsActive())
			this->gameObjectList[i]->processInput(event);
	}
}

void GameObjectManager::update(sf::Time deltaTime)
{
	background->update(deltaTime);
	//std::cout << "Delta time: " << deltaTime.asSeconds() << "\n";
	for (int i = 0; i < this->gameObjectList.size(); i++) {


		if (this->gameObjectList[i]->getIsActive())
			this->gameObjectList[i]->update(deltaTime);


	}
	textureDisplay->update(deltaTime);
	fpsCounter->update(deltaTime);

	if (!this->loaded)
	{
		this->elapsed += deltaTime.asSeconds();

		if (this->elapsed >= 1.0f)
		{
			this->elapsed = 0;
			Food* foodObj = new Food("food");
			this->addObject(foodObj);

		}
	}
}

//draws the object if it contains a sprite
void GameObjectManager::draw(sf::RenderWindow* window) {
	background->draw(window);

	for (int i = 0; i < this->gameObjectList.size(); i++) {

		if (this->gameObjectList[i]->getIsActive())
			this->gameObjectList[i]->draw(window);


		
	}
	fpsCounter->draw(window);
}

void GameObjectManager::addObject(AGameObject* gameObject)
{
	//also initialize the oject
	this->gameObjectMap[gameObject->getName()] = gameObject;
	this->gameObjectList.push_back(gameObject);
	this->gameObjectMap[gameObject->getName()]->initialize();
}

void GameObjectManager::addObjectToScene(AGameObject* gameObject)
{
	this->loadedSceneObjectMap[gameObject->getName()] = gameObject;
	this->loadedSceneObjects.push_back(gameObject);
	this->loadedSceneObjectMap[gameObject->getName()]->initialize();
}

//also frees up allocation of the object.
void GameObjectManager::deleteObject(AGameObject* gameObject)
{
	this->gameObjectMap.erase(gameObject->getName());

	int index = -1;
	for (int i = 0; i < this->gameObjectList.size(); i++) {
		if (this->gameObjectList[i] == gameObject) {
			index = i;
			break;
		}
	}

	if (index != -1) {
		this->gameObjectList.erase(this->gameObjectList.begin() + index);
	}
	
	delete gameObject;
}

void GameObjectManager::deleteObjectByName(AGameObject::String name) {
	AGameObject* object = this->findObjectByName(name);
	
	if (object != NULL) {
		this->deleteObject(object);
	}
}

void GameObjectManager::setLoadedVideo(bool loaded)
{
	this->loaded = loaded;
	this->gameObjectList.clear();
}
