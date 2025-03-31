#include "GraphicsEngine.h"

#include "Logger.h"

using namespace GDEngine;

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return this->renderSystem;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return this->textureManager;
}

MeshManager* GraphicsEngine::getMeshManager()
{
	return this->meshManager;
}

GraphicsEngine* GraphicsEngine::P_SHARED_INSTANCE = nullptr;
GraphicsEngine::GraphicsEngine()
{
	try
	{
		this->renderSystem = new RenderSystem();
	}
	catch (...)
	{
		Logger::throw_exception("RenderSystem not created successfully");
	}
	try
	{
		this->textureManager = new TextureManager();
	}
	catch (...)
	{
		Logger::throw_exception("TextureManager not created successfully");
	}
	try
	{
		this->meshManager = new MeshManager();
	}
	catch (...)
	{
		Logger::throw_exception("MeshManager not created successfully");
	}
	Logger::log(this, "Initialized");
}
GraphicsEngine::~GraphicsEngine()
{
	delete meshManager;
	delete textureManager;
	delete renderSystem;
	P_SHARED_INSTANCE = nullptr;
	Logger::log(this, "Destroyed");
}
GraphicsEngine::GraphicsEngine(const GraphicsEngine&) {}

GraphicsEngine* GraphicsEngine::getInstance() {
	return P_SHARED_INSTANCE;
}

void GraphicsEngine::initialize()
{
	if (P_SHARED_INSTANCE)
	{
		Logger::throw_exception("Graphics Engine already created");
	}
	P_SHARED_INSTANCE = new GraphicsEngine();
}

void GraphicsEngine::destroy()
{
	delete P_SHARED_INSTANCE;
}
