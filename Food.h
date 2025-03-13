#pragma once
#include "AGameObject.h"

#include "Settings.h"

class Food : public AGameObject
{
public:
	Food(String name);
	~Food();
	void initialize();
	void processInput(sf::Event event);
	void update(sf::Time deltaTime);

	void updateFrame(sf::Time elapsedTime);

	private:
		int rotDir;
		float speed;
};

