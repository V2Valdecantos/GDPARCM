#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

namespace GDEngine
{
	class SwapChain
	{
	private:
		RenderSystem* m_system = nullptr;

		IDXGISwapChain* m_swapChain;

		RenderTexture* m_renderTexture;

	private:
		friend class DeviceContext;

	public:
		SwapChain(RenderSystem* system, HWND hwnd, UINT width, UINT height);
		~SwapChain();

	public:
		void cleanRenderTarget();
		void resizeBuffers(UINT bufferCount, UINT width, UINT height);
		void createRenderTarget();
		bool present(bool vsync);

		RenderTexture* getRenderTexture();
	};
}