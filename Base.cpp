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
	this->background->setScale(2,2);
	this->initializeIcons();

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
    while (this->window->isOpen())
    {

        // check all the window's events that were triggered since the last iteration of the loop
        while (window->pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window->close();
        }




        // clear the window with black color
        window->clear(sf::Color::Black);

        // draw everything here...
        window->draw(*this->background);
		for (sf::Sprite* icon : this->icons) 
		{
			window->draw(*icon);
		}
		window->draw(*this->fpsCounter);
        // end the current frame
        window->display();

		currentTime = clock.getElapsedTime();
		this->fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
		previousTime = currentTime;
		this->fpsCounter->setString("FPS: " + std::to_string((this->fps)));

		deltaClock.restart();
		
    }
}

void Base::initializeIcons()
{
	float x = 5;
	float y = 0;
	std::string name = "";

	for (int i = 0; i <= 149; i++)
	{
		sf::Sprite* icon = new sf::Sprite();
		if (i < 10)
		{
			name = "00" + std::to_string(i);
			icon->setTexture(*(TextureManager::getInstance()->getTextureByName(name)));
			icon->setPosition(x, y);
			this->icons.push_back(icon);
			x += 50;
		}
		else if (i >= 10 && i < 100)
		{
			name = "0" + std::to_string(i);
			icon->setTexture(*(TextureManager::getInstance()->getTextureByName(name)));
			icon->setPosition(x, y);
			this->icons.push_back(icon);
			x += 50;
		}
		else if (i >= 100)
		{
			name = std::to_string(i);
			icon->setTexture(*(TextureManager::getInstance()->getTextureByName(name)));
			icon->setPosition(x, y);
			this->icons.push_back(icon);
			x += 50;
		}

		if (i % 37 == 0 && i != 0) 
		{
			x = 5;
			y += 50;
		}

		icon->setScale(0.75f, 0.75f);
		std::cout << "Initialized tile" + name << std::endl;
		std::cout << this->icons.size() << std::endl;
	}
}