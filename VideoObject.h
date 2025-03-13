#pragma once
#include "AGameObject.h"
class VideoObject : public AGameObject
{
	public:
		VideoObject(String name);
		void initialize();
		void processInput(sf::Event event);
		void update(sf::Time deltaTime);

		void updateFrame(sf::Time elapsedTime);

	private:
		sf::Time updateTime;
		int currentFrame = 0;
};

