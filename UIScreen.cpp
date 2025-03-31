#include "UIScreen.h"

using namespace GDEngine;

UIScreen::UIScreen(std::string name)
{
	this->name = name;
}

UIScreen::~UIScreen()
{
	
}

std::string UIScreen::getName()
{
	return this->name;
}

void UIScreen::setActive(bool isActive)
{
	this->isActive = true;
}


