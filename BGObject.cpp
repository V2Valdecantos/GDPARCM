#include "BGObject.h"
#include <iostream>
#include "TextureManager.h"
#include "BaseRunner.h"

BGObject::BGObject(String name) : AGameObject(name)
{
}

void BGObject::initialize()
{
	std::cout << "Declared as " << this->getName() << "\n";

	//assign texture
	this->sprite = new sf::Sprite();
	sf::Texture* texture = TextureManager::getInstance()->getFromTextureMap("bg", 0);
	texture->setRepeated(true);
	this->sprite->setTexture(*texture);
	sf::Vector2u textureSize = this->sprite->getTexture()->getSize();
	//make BG height x k to emulate repeating BG.
	//this->sprite->setTextureRect(sf::IntRect(0,0,BaseRunner::WINDOW_WIDTH, BaseRunner::WINDOW_HEIGHT)); 
	this->setPosition(0, -10);
	this->setScale(0.86f, 0.86f);
}

void BGObject::processInput(sf::Event event)
{
}

void BGObject::update(sf::Time deltaTime)
{
	////make BG scroll slowly
	//sf::Vector2f position = this->getPosition();
	//position.y += this->SPEED_MULTIPLIER * deltaTime.asSeconds();
	//this->setPosition(position.x, position.y);

	//sf::Vector2f localPos = this->sprite->getPosition();
	//if (localPos.y * deltaTime.asSeconds() > 0) {
	//	//reset position
	//	this->setPosition(0, -BaseRunner::WINDOW_HEIGHT * 7);
	//}
	//else {
	//	
	//}
}
