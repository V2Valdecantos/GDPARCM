#include "Player.h"
#include "TextureManager.h"
Player::Player() : AGameObject("Player")
{
}

Player::~Player()
{
}

void Player::initialize()
{
	this->sprite = new sf::Sprite();
	sf::Texture* texture = TextureManager::getInstance()->getFromTextureMap("Eagle", 0);
	texture->setRepeated(true);
	this->sprite->setTexture(*texture);
	sf::Vector2u textureSize = this->sprite->getTexture()->getSize();
	//make BG height x k to emulate repeating BG.
	//this->sprite->setTextureRect(sf::IntRect(0,0,BaseRunner::WINDOW_WIDTH, BaseRunner::WINDOW_HEIGHT)); 
	this->setPosition(500, 500);
	this->setScale(3, 3);
}

void Player::update(sf::Time deltaTime)
{
	if (this->isMovingLeft)
		this->posX -= 1 * deltaTime.asMilliseconds();

	if (this->isMovingRight)
		this->posX += 1 * deltaTime.asMilliseconds();
}

void Player::processInput(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
		this->onKeyDown(event.key);

	if (event.type == sf::Event::KeyReleased)
		this->onKeyUp(event.key);
}

void Player::onKeyDown(sf::Event::KeyEvent key)
{
	switch (key.code) 
	{
		case sf::Keyboard::A:
			this->isMovingLeft = true;
			break;

		case sf::Keyboard::D:
			this->isMovingRight = true;
			break;

	}
}

void Player::onKeyUp(sf::Event::KeyEvent key)
{
	switch (key.code)
	{
		case sf::Keyboard::A:
			this->isMovingLeft = false;
			break;

		case sf::Keyboard::D:
			this->isMovingRight = false;
			break;
	}
}
