#pragma once
#include <vector>
#include <unordered_map>
#include "SFML/Graphics.hpp"
#include "thread"
#include <iostream>

class TextureManager
{
	public:
		void initialize();
		sf::Texture* getTextureByName(std::string name);
	private:
		std::unordered_map<std::string, sf::Texture*> textures;


	private:
		static TextureManager* P_SHARED_INSTANCE;

	private:
		TextureManager();
		~TextureManager();
		TextureManager(TextureManager const&);
		TextureManager& operator = (TextureManager const&);

	public:
		static TextureManager* getInstance();
		static void destroy();
};

