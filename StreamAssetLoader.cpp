#include "StreamAssetLoader.h"
#include <iostream>
#include <random>

#include "TextureManager.h"
#include "StringUtils.h"
#include "IExecutionEvent.h"
#include "AGameObject.h"

StreamAssetLoader::StreamAssetLoader(String path, IExecutionEvent* executionEvent)
{
	this->path = path;
	this->execEvent = executionEvent;
}

StreamAssetLoader::~StreamAssetLoader()
{
	std::cout << "Destroying stream asset loader. " << std::endl;
}

void StreamAssetLoader::run()
{
	std::cout << "Running stream asset loader " << std::endl;
	//simulate loading of very large file
	//std::random_device seeder;
	//std::mt19937 engine(seeder());
	//std::uniform_int_distribution<int> dist(0, 400);
	//IETThread::sleep(dist(engine));

	std::vector<String> tokens = StringUtils::split(path, '/');
	String assetName = StringUtils::split(tokens[tokens.size() - 1], '.')[0];

	TextureManager::getInstance()->instantiateAsTexture(path, assetName, true);

	std::cout << "[TextureManager] Loaded streaming texture: " << assetName << std::endl;

	//make game object
	//SceneObject* object = new SceneObject();

	//this->execEvent->onFinishedExecution();
	//delete after being done
	delete this;
}

void StreamAssetLoader::onStartTask()
{
	std::cout << "Running stream asset loader " << std::endl;
	//simulate loading of very large file
	std::random_device seeder;
	std::mt19937 engine(seeder());
	std::uniform_int_distribution<int> dist(1000, 4000);
	IETThread::sleep(dist(engine));

	std::vector<String> tokens = StringUtils::split(path, '/');
	String assetName = StringUtils::split(tokens[tokens.size() - 1], '.')[0];

	//TextureManager::getInstance()->instantiateAsTexture(path, assetName, true);
	TextureManager::getInstance()->instantiateAsVideoFrame(path, assetName, std::stoi(assetName) - 1);

	std::cout << "[TextureManager] Loaded video frame: " << assetName << std::endl;

	this->execEvent->onFinishedExecution();
	//delete after being done
	delete this;
}
