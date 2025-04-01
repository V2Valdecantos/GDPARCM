#include "TextureLibrary.h"
#include "GraphicsEngine.h"

using namespace GDEngine;

TextureMap GDEngine::TextureLibrary::getTextureLibrary()
{
	return this->texLibrary;
}

Texture* GDEngine::TextureLibrary::getTexture(TextureName ETextureName)
{
	return this->texLibrary[ETextureName];
}

GDEngine::TextureLibrary::TextureLibrary()
{
	P_SHARED_INSTANCE = this;

	Texture* temp;

	temp = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(L"assets/textures/default.jpg");
	this->texLibrary[TextureName::DEFAULT] = temp;
	temp = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(L"assets/textures/wood.jpg");
	this->texLibrary[TextureName::WOOD] = temp;
	temp = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(L"assets/textures/brick.png");
	this->texLibrary[TextureName::BRICK] = temp;
	temp = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(L"assets/textures/dlsu.png");
	this->texLibrary[TextureName::DLSU] = temp;
	temp = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(L"assets/textures/sand.jpg");
	this->texLibrary[TextureName::SAND] = temp;
	temp = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(L"assets/textures/wall.jpg");
	this->texLibrary[TextureName::WALL] = temp;
	temp = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(L"assets/textures/house_wood.jpg");
	this->texLibrary[TextureName::HOUSE_WOOD] = temp;
	temp = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(L"assets/textures/pxfuel.jpg");
	this->texLibrary[TextureName::PXFUEL] = temp;
	temp = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(L"assets/textures/barrel.jpg");
	this->texLibrary[TextureName::BARREL] = temp;
}

GDEngine::TextureLibrary::~TextureLibrary()
{
}

TextureLibrary* TextureLibrary::P_SHARED_INSTANCE = nullptr;

GDEngine::TextureLibrary* GDEngine::TextureLibrary::getInstance()
{
	if (P_SHARED_INSTANCE == NULL) 
	{
		P_SHARED_INSTANCE = new TextureLibrary();
	}

	return P_SHARED_INSTANCE;
}
