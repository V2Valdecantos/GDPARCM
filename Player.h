#pragma once
#include "AGameObject.h"
class Player : public AGameObject
{
	public:
		Player();
		~Player();

		virtual void initialize() override;
		virtual void update(sf::Time deltaTime) override;
		virtual void processInput(sf::Event event) override;
		virtual void onKeyDown(sf::Event::KeyEvent key) override;
		virtual void onKeyUp(sf::Event::KeyEvent key) override;
	private:
		int health;

		bool isMovingLeft = false;
		bool isMovingRight = false;
		bool isMovingUp = false;
		bool isMovingDown = false;

};

