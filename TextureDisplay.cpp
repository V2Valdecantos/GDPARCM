#include "TextureDisplay.h"
#include <iostream>
#include "TextureManager.h"
#include "BaseRunner.h"
#include "GameObjectManager.h"
#include "IconObject.h"
#include "VideoObject.h"
TextureDisplay::TextureDisplay(): AGameObject("TextureDisplay")
{
	
}

void TextureDisplay::initialize()
{
	
}

void TextureDisplay::processInput(sf::Event event)
{
	
}

void TextureDisplay::update(sf::Time deltaTime)
{
	this->ticks += BaseRunner::TIME_PER_FRAME.asMilliseconds();
	if (this->streamingType == StreamingType::BATCH_LOAD && !this->startedStreaming && this->ticks > this->STREAMING_LOAD_DELAY)
	{
		this->startedStreaming = true;
		this->ticks = 0.0f;
		TextureManager::getInstance()->loadStreamingAssets();
	}
	else if (this->streamingType == StreamingType::SINGLE_STREAM && this->ticks > this->STREAMING_LOAD_DELAY)
	{
		TextureManager::getInstance()->loadSingleStreamAsset(this->numDisplayed, this);
		this->numDisplayed++;
		this->ticks = 0.0f;
		//this->totalFrames = TextureManager::getInstance()->getTotalFrames();
	}

	if (!this->isDone && this->framesLoaded == this->totalFrames) 
	{
		this->isDone = true;
		this->startVideo();
		
	}
}

void TextureDisplay::onFinishedExecution()
{
	//this->spawnObject(); //executes spawn once the texture is ready.
	this->guard.lock();
	this->framesLoaded++;
	this->guard.unlock();
}

void TextureDisplay::spawnObject()
{
	this->guard.lock();
	
	String objectName = "Icon_" + to_string(this->iconList.size());
	IconObject* iconObj = new IconObject(objectName, this->iconList.size());
	this->iconList.push_back(iconObj);

	//set position
	int IMG_WIDTH = 256; int IMG_HEIGHT = 256;
	float x = this->columnGrid * IMG_WIDTH;
	float y = this->rowGrid * IMG_HEIGHT;
	//iconObj->setPosition(x, y);
	iconObj->setPosition(0, 0);
	std::cout << "Set position: " << x << " " << y << std::endl;

	this->columnGrid++;
	if(this->columnGrid == this->MAX_COLUMN)
	{
		this->columnGrid = 0;
		this->rowGrid++;
	}
	GameObjectManager::getInstance()->addObject(iconObj);

	this->guard.unlock();
}

void TextureDisplay::startVideo()
{
	GameObjectManager::getInstance()->setLoadedVideo(true);
	VideoObject* video = new VideoObject("cooking");
	GameObjectManager::getInstance()->addObject(video);

}
