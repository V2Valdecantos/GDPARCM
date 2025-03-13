#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Input.h"

class AGameObject: sf::NonCopyable, public Input
{
	public:
		typedef std::string String;
		AGameObject(String name);
		~AGameObject();
		virtual void initialize() = 0;
		virtual void processInput(sf::Event event) = 0;
		virtual void update(sf::Time deltaTime) = 0;
		virtual void draw(sf::RenderWindow* targetWindow);
		String getName();

		virtual void onKeyDown(sf::Event::KeyEvent key);
		virtual void onKeyUp(sf::Event::KeyEvent key);

		virtual void setPosition(float x, float y);
		virtual void setScale(float x, float y);
		
		virtual void setActive(bool active);
		virtual sf::FloatRect getLocalBounds();
		sf::FloatRect getGlobalBounds();
		virtual sf::Vector2f getPosition();
		virtual sf::Vector2f getScale();

		virtual bool getIsActive();

	protected:
		String name;
		sf::Sprite* sprite;
		sf::Texture* texture;

		bool isActive = true;

		float posX = 0.0f; float posY = 0.0f;
		float scaleX = 1.0f; float scaleY = 1.0f;
};

