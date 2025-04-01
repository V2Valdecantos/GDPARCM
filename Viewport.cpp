#include "Viewport.h"

#include <exception>

#include "RenderSystem.h"
#include "DeviceContext.h"
#include "Logger.h"
#include "RasterizerState.h"

using namespace GDEngine;

Viewport::Viewport(RenderSystem* system, FLOAT topLeftX, FLOAT topLeftY, FLOAT width, FLOAT height, FLOAT minDepth, FLOAT maxDepth) : m_system(system)
{
	m_viewport = {};

	setPosition(topLeftX, topLeftY);
	setSize(width, height);
	setDepth(minDepth, maxDepth);

	Logger::log(this, "Initialized");
}

Viewport::~Viewport()
{
}

void Viewport::setPosition(FLOAT topLeftX, FLOAT topLeftY)
{
	m_viewport.TopLeftX = topLeftX;
	m_viewport.TopLeftY = topLeftY;
}

void Viewport::setSize(FLOAT width, FLOAT height)
{
	m_viewport.Width = width;
	m_viewport.Height = height;
}

void Viewport::setDepth(FLOAT minDepth, FLOAT maxDepth)
{
	m_viewport.MinDepth = minDepth;
	m_viewport.MaxDepth = maxDepth;
}