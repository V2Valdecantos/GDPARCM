#pragma once

#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"

namespace GDEngine
{
	class GraphicsEngine
	{
	private:
		RenderSystem* renderSystem = nullptr;
		TextureManager* textureManager = nullptr;
		MeshManager* meshManager = nullptr;

	public:
		RenderSystem* getRenderSystem();
		TextureManager* getTextureManager();
		MeshManager* getMeshManager();

	private:
		static GraphicsEngine* P_SHARED_INSTANCE;

	private:
		GraphicsEngine();
		~GraphicsEngine();
		GraphicsEngine(GraphicsEngine const&);
		GraphicsEngine& operator = (GraphicsEngine const&);

	public:
		static GraphicsEngine* getInstance();
		static void initialize();
		static void destroy();
	};
}
