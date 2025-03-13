#include "BaseRunner.h"

#include <iostream>

#include  "GameObjectManager.h"
#include "BGObject.h"
#include "TextureManager.h"
#include "TextureDisplay.h"
#include "SFXManager.h"
#include "FPSCounter.h"
#include "Player.h"
#include "Food.h"


/// <summary>
/// This demonstrates a running parallax background where after X seconds, a batch of assets will be streamed and loaded.
/// </summary>

const float FRAME_RATE = FRAMERATE;
const sf::Time BaseRunner::TIME_PER_FRAME = sf::seconds(1.0f / FRAME_RATE);
BaseRunner* BaseRunner::sharedInstance = NULL;

BaseRunner::BaseRunner() :
	window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Valdecantos - Interactive Loading Screen", sf::Style::Close) {

	sharedInstance = this;
	this->window.setFramerateLimit(int(FRAME_RATE));
	
	//load initial textures
	TextureManager::getInstance()->loadFromAssetList();


	SFXManager::getInstance()->loadAll();
	SFXManager::getInstance()->getSound(SFXType::BACKGROUND)->setLoop(true);
	SFXManager::getInstance()->getSound(SFXType::BACKGROUND)->setVolume(100);
	SFXManager::getInstance()->getSound(SFXType::BACKGROUND)->play();

	//load objects
	BGObject* bgObject = new BGObject("BGObject");
	//GameObjectManager::getInstance()->addObject(bgObject);
	bgObject->initialize();
	GameObjectManager::getInstance()->background = bgObject;

	Player* player = new Player;
	GameObjectManager::getInstance()->addObject(player);
	
	Food* foodObj = new Food("food");
	GameObjectManager::getInstance()->addObject(foodObj);

	TextureDisplay* display = new TextureDisplay();
	//GameObjectManager::getInstance()->addObject(display);
	display->initialize();
	GameObjectManager::getInstance()->textureDisplay = display;

	FPSCounter* fpsCounter = new FPSCounter();
	//GameObjectManager::getInstance()->addObject(fpsCounter);
	fpsCounter->initialize();
	GameObjectManager::getInstance()->fpsCounter = fpsCounter;
}

void BaseRunner::run() {
	sf::Clock clock;
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time currentTime;
	while (this->window.isOpen())
	{
		currentTime = clock.getElapsedTime();
		float deltaTime = currentTime.asSeconds() - previousTime.asSeconds();
		this->fps = floor(1.0f / deltaTime);

		processEvents();
		update(sf::seconds(1.0f / this->fps));
		render();
		
		previousTime = currentTime;
		
		
	}
}

BaseRunner* BaseRunner::getInstance()
{
	return sharedInstance;
}

int BaseRunner::getFPS() const
{
	return int(this->fps);
}

void BaseRunner::processEvents()
{
	sf::Event event;
	if (this->window.pollEvent(event)) {
		switch (event.type) {
		
		default: GameObjectManager::getInstance()->processInput(event); break;
			
		case sf::Event::Closed:
			this->window.close();
			break;

		}
	}
}

void BaseRunner::processInput(sf::Event::KeyEvent key) 
{
	switch (key.code == sf::Keyboard::W) 
	{

	}
}
void BaseRunner::update(sf::Time elapsedTime) {
	GameObjectManager::getInstance()->update(elapsedTime);
	
}

void BaseRunner::render() {
	this->window.clear();
	GameObjectManager::getInstance()->draw(&this->window);
	this->window.display();
}