#include "TextureManager.h"


TextureManager* TextureManager::P_SHARED_INSTANCE = NULL;

TextureManager* TextureManager::getInstance()
{
	if (P_SHARED_INSTANCE == NULL) 
	{
		P_SHARED_INSTANCE = new TextureManager();
	}

	return P_SHARED_INSTANCE;
}

void TextureManager::initialize()
{
	P_SHARED_INSTANCE = this;

	//initialize texture map
	//bg
	sf::Texture* bgtex = new sf::Texture;
	bgtex->loadFromFile("assets\\Textures\\Desert.png");
	bgtex->setRepeated(true);
	this->textures["Desert"] = bgtex;
	//icons
	std::string name = "";

	for (int i = 0; i <= 479; i++) 
	{
		sf::Texture* tex = new sf::Texture;
		if (i < 10 && i < 100) 
		{
			name = "00" + std::to_string(i);
			tex->loadFromFile("assets\\Streaming\\tile" + name + ".png");
			this->textures[name] = tex;
		}
		else if (i < 100 && i >= 10)
		{
			name = "0" + std::to_string(i);
			tex->loadFromFile("assets\\Streaming\\tile" + name + ".png");
			this->textures[name] = tex;
		} 
		else if (i >= 100) 
		{
			name = std::to_string(i);
			tex->loadFromFile("assets\\Streaming\\tile" + name + ".png");
			this->textures[name] = tex;
		}
	}

}

sf::Texture* TextureManager::getTextureByName(std::string name)
{
	sf::Texture* texture = this->textures[name];
	return texture;
}

TextureManager::TextureManager()
{
	this->initialize();
}

TextureManager::~TextureManager()
{
}
