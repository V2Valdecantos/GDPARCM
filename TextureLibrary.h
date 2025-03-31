#pragma once
#include <string>
#include <unordered_map>

#include "Prerequisites.h"
#include "TextureManager.h"
#include "Texture.h"

namespace GDEngine {

	enum TextureName
	{
		DEFAULT = 0, WOOD, BRICK, DLSU, SAND, WALL, HOUSE_WOOD, PXFUEL, BARREL
	};

	typedef std::unordered_map<TextureName, Texture*> TextureMap;

	class TextureLibrary
	{
		private:
			TextureMap texLibrary;

		public:
			TextureMap getTextureLibrary();
			Texture* getTexture(TextureName ETextureName);

		private:
			static TextureLibrary* P_SHARED_INSTANCE;

		private:
			TextureLibrary();
			~TextureLibrary();
			TextureLibrary(TextureLibrary const&);
			TextureLibrary& operator = (TextureLibrary const&);

		public:
			static TextureLibrary* getInstance();

	};
}
