#pragma once

#include "ResourceManager.h"

namespace GDEngine
{
	class TextureManager : public ResourceManager
	{
	public:
		Texture* createTextureFromFile(const wchar_t* filePath);

	protected:
		virtual Resource* createResourceFromFileConcrete(const wchar_t* filePath);
		friend class GraphicsEngine;

	private:
		TextureManager();
		~TextureManager() override;
	};
}