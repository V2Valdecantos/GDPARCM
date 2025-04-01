#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

namespace GDEngine
{
	class Viewport
	{
	private:
		RenderSystem* m_system = nullptr;
		D3D11_VIEWPORT m_viewport;

	private:
		friend class DeviceContext;

	public:
		Viewport(RenderSystem* system, FLOAT topLeftX, FLOAT topLeftY, FLOAT width, FLOAT height, FLOAT minDepth,
			FLOAT maxDepth);
		~Viewport();

	public:
		void setPosition(FLOAT topLeftX, FLOAT topLeftY);
		void setSize(FLOAT width, FLOAT height);
		void setDepth(FLOAT minDepth, FLOAT maxDepth);
		
	};
}