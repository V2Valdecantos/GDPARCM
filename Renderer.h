#pragma once
#include "AComponent.h"

namespace GDEngine {
	class ARenderer : public AComponent
	{
	public:
		ARenderer(std::string name, AGameObject* owner);
		ARenderer(std::string guid, std::string name, AGameObject* owner);
		~ARenderer() override;

		virtual void perform(float deltaTime) override;
	};
}

