#include "Renderer.h"

namespace GDEngine
{
	ARenderer::ARenderer(std::string name, AGameObject* owner) : AComponent(name, ComponentType::Renderer, owner)
	{
	}

	ARenderer::ARenderer(std::string guid, std::string name, AGameObject* owner) : AComponent(guid, name, ComponentType::Renderer, owner)
	{
	}

	ARenderer::~ARenderer()
	{
		AComponent::~AComponent();
	}

	void ARenderer::perform(float deltaTime)
	{
		
	}
}
