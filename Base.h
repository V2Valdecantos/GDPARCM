#pragma once

#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <optional>
#include <math.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "TextureManager.h"
class Base
{
	public:
		Base();
		~Base();

		
		void run();		
		void initializeIcons(int iconIndex);
		void initThreads();


	private:
		sf::RenderWindow* window;
		sf::Sprite* background;
		std::vector<sf::Sprite*> icons;
		sf::Text* fpsCounter;
		
		float frameRate = 60.0f;
		int fps;
		sf::Time updateTime;
		sf::Time timePerFrame;
		float height = 1080;
		float width = 1920;
		int iconIndex = 0;
		int iconX, iconY;
		bool canDraw = false;

};

