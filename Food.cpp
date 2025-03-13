#include "Food.h"
#include <random>
#include "SFML/Graphics.hpp"
#include "StringUtils.h"

#include "TextureManager.h"
#include "GameObjectManager.h"

Food::Food(String name) : AGameObject(name)
{
}

Food::~Food()
{
}

void Food::initialize()
{
	this->sprite = new sf::Sprite;

	this->scaleX = 0.2f;
	this->scaleY = 0.2f;
	this->sprite->setScale(0.2f, 0.2f);
	//random x pos and type
	std::random_device seeder;
	std::mt19937 engine(seeder());
	std::uniform_int_distribution<int> pos(0, WINDOW_WIDTH - (394 * this->scaleX));
	std::uniform_int_distribution<int> type(1, 3);
	std::uniform_int_distribution<int> rot(0, 1);
	std::uniform_int_distribution<int> sp(10, 100);
	this->rotDir = rot(engine);
	this->speed = sp(engine);
	int foodIndex = type(engine);
	std::string textureName = "";

	if (foodIndex == 1)
		textureName = "meat";
	if (foodIndex == 2)
		textureName = "herb";
	if (foodIndex == 3)
		textureName = "fish";

	sf::Texture* texture = TextureManager::getInstance()->getFromTextureMap(textureName, 0);
	this->sprite->setTexture(*texture);

	this->sprite->setOrigin(this->sprite->getLocalBounds().width / 2, this->sprite->getLocalBounds().height / 2);

	this->posX = pos(engine);
	this->posY = -394;
	std::cout << "Initialized Food" << std::endl;
}

void Food::processInput(sf::Event event)
{
}

void Food::update(sf::Time deltaTime)
{
	this->posY = this->posY + (98 * deltaTime.asSeconds());

	if (this->rotDir == 0)
	{
		this->sprite->rotate(-this->speed * deltaTime.asSeconds());
	}
	else 
	{
		this->sprite->rotate(this->speed * deltaTime.asSeconds());
	}

	if (this->posY >= WINDOW_HEIGHT + 400) 
	{
		GameObjectManager::getInstance()->deleteObject(this);
	}
}

void Food::updateFrame(sf::Time elapsedTime)
{
}
