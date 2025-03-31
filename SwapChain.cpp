#include "SwapChain.h"

#include <exception>

#include "RenderSystem.h"
#include "RenderTexture.h"
#include "Logger.h"

using namespace GDEngine;

SwapChain::SwapChain(RenderSystem* system, HWND hwnd, UINT width, UINT height) : m_system(system)
{
	ID3D11Device* device = this->m_system->m_D3DDevice;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;

	// Create the swap chain for the window indicated by HWND parameter
	HRESULT hr = this->m_system->m_dxgiFactory->CreateSwapChain(device, &desc, &m_swapChain);

	if (!Logger::log(this, hr))
		Logger::throw_exception("SwapChain not created successfully");

	try
	{
		m_renderTexture = new RenderTexture();
	}
	catch(...) {}
}

SwapChain::~SwapChain()
{
	delete m_renderTexture;
	m_swapChain->Release();
}

void SwapChain::cleanRenderTarget()
{
	if (m_renderTexture->m_renderTargetView)
	{
		m_renderTexture->m_renderTargetView->Release();
		m_renderTexture->m_renderTargetView = nullptr;
	}
}

void SwapChain::resizeBuffers(UINT bufferCount, UINT width, UINT height)
{
	m_renderTexture->resizeResources(width, height);
	m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
}

void SwapChain::createRenderTarget()
{
	ID3D11Texture2D* buffer = NULL;
	HRESULT result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (!Logger::log(this, result))
		Logger::throw_exception("Swap Chain Fail");

	result = this->m_system->m_D3DDevice->CreateRenderTargetView(buffer, NULL, &m_renderTexture->m_renderTargetView);
	if (!Logger::log(this, result))
		Logger::throw_exception("Render Target View not created successfully.");

	buffer->Release();
}

bool SwapChain::present(bool vsync)
{
	m_swapChain->Present(vsync, NULL);
	return true;
}

RenderTexture* SwapChain::getRenderTexture()
{
	return this->m_renderTexture;
}
