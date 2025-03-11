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
	sf::Texture* texture = TextureManager::getInstance()->getFromTextureMap("1", 0);
	texture->setRepeated(true);
	this->sprite->setTexture(*texture);
	sf::Vector2u textureSize = this->sprite->getTexture()->getSize();
	//make BG height x k to emulate repeating BG.
	//this->sprite->setTextureRect(sf::IntRect(0,0,BaseRunner::WINDOW_WIDTH, BaseRunner::WINDOW_HEIGHT)); 
	this->setPosition(500, 900);
	this->setScale(0.1, 0.1);
}

void Player::update(sf::Time deltaTime)
{

	//Player Movement
	if (this->isMovingLeft)
		this->posX -= 0.7 * deltaTime.asMilliseconds();

	if (this->isMovingRight)
		this->posX += 0.7 * deltaTime.asMilliseconds();

	if (this->isMovingUp)
		this->posY -= 0.7 * deltaTime.asMilliseconds();

	if (this->isMovingDown)
		this->posY += 0.7 * deltaTime.asMilliseconds();

	//Bounds Checking
	//X
	if (this->posX <= 0)
		this->posX = 0;
	if (this->posX + (this->getLocalBounds().width * this->scaleX) >= 1920)
		this->posX = 1920 - (this->getLocalBounds().width * this->scaleX);
	//Y
	if (this->posY <= 0)
		this->posY = 0;
	if (this->posY + (this->getLocalBounds().height * this->scaleX) >= 1080)
		this->posY = 1080 - (this->getLocalBounds().height * this->scaleX);
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
		case sf::Keyboard::W:
			this->isMovingUp = true;
			break;

		case sf::Keyboard::A:
			this->isMovingLeft = true;
			break;

		case sf::Keyboard::S:
			this->isMovingDown = true;
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
	case sf::Keyboard::W:
		this->isMovingUp = false;
		break;

	case sf::Keyboard::A:
		this->isMovingLeft = false;
		break;

	case sf::Keyboard::S:
		this->isMovingDown = false;
		break;

	case sf::Keyboard::D:
		this->isMovingRight = false;
		break;
	}
}
