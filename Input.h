#pragma once
#include "SFML/Graphics.hpp"
class Input 
{
	public:
		virtual void onKeyDown(sf::Event::KeyEvent key) = 0;
		virtual void onKeyUp(sf::Event::KeyEvent key) = 0;
};