#include "Base.h"


Base::Base()
{
	//config
	this->frameRate = 60.0f;
	this->timePerFrame = sf::seconds(1.0f / frameRate);
	this->height = 1080;
	this->width = 1920;
	//window and icons
	this->window = new sf::RenderWindow(sf::VideoMode({ 1920, 1080 }), "Vito Valdecantos - Asset Loader");
    window->setFramerateLimit(this->frameRate);
    this->background = new sf::Sprite();
    this->background->setTexture(*(TextureManager::getInstance()->getTextureByName("Desert")));
	this->background->setTextureRect(sf::IntRect(0,0, 1920, 1080 * 8));
	this->background->setPosition(0, -1080 * 7);
	this->iconX = 5;
	this->iconY = 0;

	//fps counter
	this->fpsCounter = new sf::Text;
	sf::Font* font = new sf::Font();
	font->loadFromFile("assets\\Sansation.ttf");
	
	this->fpsCounter->setFont(*font);
	this->fpsCounter->setPosition(width - 150, height - 70);
	this->fpsCounter->setOutlineColor(sf::Color(1, 1, 1));
	this->fpsCounter->setOutlineThickness(2.5f);
	this->fpsCounter->setCharacterSize(35);


	
}

Base::~Base()
{
}

void Base::run()
{

    sf::Event event;
	sf::Clock clock;
	sf::Clock deltaClock;
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time currentTime;
	std::thread initThread(&Base::initThreads, this);
	initThread.detach();

	while (this->window->isOpen())
	{

		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
		}

	
		//BG Scroll
		sf::Vector2f pos = this->background->getPosition();
		pos.y += 100000 * deltaClock.getElapsedTime().asSeconds();
		this->background->setPosition(pos.x, pos.y);

		if (this->background->getPosition().y > 0)
		{
			this->background->setPosition(0, -1080 * 7);
		}

		//Icons
		//std::cout << iconClock.getElapsedTime().asSeconds() << std::endl;

		//if (iconClock.getElapsedTime().asSeconds() > 0.2f)
		//{
		//	if (this->iconIndex <= 479)
		//	{
		//		//this->initializeIcons(this->iconIndex);

		//		this->iconIndex++;
		//		iconClock.restart();
		//	}
		//}

		//Draw
		window->clear(sf::Color::Black);

		window->draw(*this->background);
		for (sf::Sprite* icon : this->icons)
		{
			if (icon != NULL)
				window->draw(*icon);
		}
		window->draw(*this->fpsCounter);
		window->display();

		//FPS Counter
		currentTime = clock.getElapsedTime();
		this->fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
		previousTime = currentTime;
		this->fpsCounter->setString("FPS: " + std::to_string((this->fps)));
	
		deltaClock.restart();
    }
}

void Base::initializeIcons(int iconIndex)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000 + iconIndex * 1000));

	std::string name = "";

	sf::Sprite* icon = new sf::Sprite();
	if (iconIndex < 10)
	{
		name = "00" + std::to_string(iconIndex);
		icon->setTexture(*(TextureManager::getInstance()->getTextureByName(name)));
		icon->setPosition(this->iconX, this->iconY);
		this->icons.push_back(icon);
		this->iconX += 50;
	}
	else if (iconIndex >= 10 && iconIndex < 100)
	{
		name = "0" + std::to_string(iconIndex);
		icon->setTexture(*(TextureManager::getInstance()->getTextureByName(name)));
		icon->setPosition(this->iconX, this->iconY);
		this->icons.push_back(icon);
		this->iconX += 50;
	}
	else if (iconIndex >= 100)
	{
		name = std::to_string(iconIndex);
		icon->setTexture(*(TextureManager::getInstance()->getTextureByName(name)));
		icon->setPosition(this->iconX, this->iconY);
		this->icons.push_back(icon);
		this->iconX += 50;
	}

	if (iconIndex % 37 == 0 && iconIndex != 0)
	{
		this->iconX = 5;
		this->iconY += 50;
	}

	icon->setScale(0.75f, 0.75f);



}

void Base::initThreads()
{
	for (int i = 0; i < 478; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		std::thread iconThread(&Base::initializeIcons, this, i);
		iconThread.detach();
	}
}