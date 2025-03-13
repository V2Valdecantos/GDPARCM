#include "VideoObject.h"
#include "TextureManager.h"
#include "SFXManager.h"
#include "GameObjectManager.h"
VideoObject::VideoObject(String name) : AGameObject(name)
{
}

void VideoObject::initialize()
{
	this->sprite = new sf::Sprite();
	sf::Texture* texture = TextureManager::getInstance()->getVideoFrame(0);
	this->sprite->setTexture(*texture);
	this->setPosition(0, 0);
	currentFrame = 0;
	SFXManager::getInstance()->getSound(SFXType::BACKGROUND)->stop();
	SFXManager::getInstance()->getSound(SFXType::VIDEO)->setLoop(true);
	SFXManager::getInstance()->getSound(SFXType::VIDEO)->play();
	
}

void VideoObject::processInput(sf::Event event)
{
}

void VideoObject::update(sf::Time deltaTime)
{
	this->updateFrame(deltaTime);
}

void VideoObject::updateFrame(sf::Time elapsedTime)
{
	this->updateTime += elapsedTime;
	if (this->updateTime >= sf::seconds(1.0f / 35.0f))
	{
		this->updateTime = sf::seconds(0.0f);

		currentFrame++;

		if (currentFrame >= TextureManager::getInstance()->getFrameCount() - 1) 
		{
			currentFrame = 0;
			//SFXManager::getInstance()->getSound(SFXType::VIDEO)->stop();
			//GameObjectManager::getInstance()->deleteObject(this);
		}
			sf::Texture* texture = TextureManager::getInstance()->getVideoFrame(currentFrame);
			this->sprite->setTexture(*texture);

	}
}
