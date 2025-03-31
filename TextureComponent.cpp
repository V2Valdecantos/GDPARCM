#include "TextureComponent.h"

#include "GameObject.h"
#include "GraphicsEngine.h"

using namespace GDEngine;

TextureComponent::TextureComponent(std::string name, AGameObject* owner) : AComponent(name, Tex, owner) 
{
	Logger::log("Created new Texture Component");
	this->setTexture(TextureName::DEFAULT);
	this->updateTexture();
}

TextureComponent::TextureComponent(std::string guid, std::string name, AGameObject* owner) : AComponent(guid, name, Tex, owner)\
{
	Logger::log("Created new Texture Component");
	this->setTexture(TextureName::DEFAULT);
	this->updateTexture();
}

GDEngine::TextureComponent::~TextureComponent()
{
	this->texture = nullptr;
}

void GDEngine::TextureComponent::setTexture(TextureName texName)
{

	this->texName = texName;
	this->texture = TextureLibrary::getInstance()->getTexture(texName);
	if (this->texture == nullptr)
	{
		this->texture = TextureLibrary::getInstance()->getTexture(TextureName::DEFAULT);
		this->updateTexture();
	}
	else
	{
		this->updateTexture();
	}
		
}

void GDEngine::TextureComponent::removeTexture()
{
	this->filePath = "";
	this->setTexture(TextureName::DEFAULT);
	this->updateTexture();
	this->getOwner()->setHasTexture(false);
}

void GDEngine::TextureComponent::updateTexture()
{
	this->getOwner()->setTexture(this->texture);
	this->getOwner()->setHasTexture(true);
	//this->getOwner()->toggleTexture();
}

TextureName TextureComponent::getTexName()
{
	return this->texName;
}

Texture* GDEngine::TextureComponent::getTexture()
{
	return this->texture;
}
