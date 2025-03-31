#include "RenderTexture.h"

#include "GraphicsEngine.h"
#include "Logger.h"

using namespace GDEngine;

RenderTexture::RenderTexture() : m_width(0), m_height(0)
{
    this->m_renderTarget = NULL;
    resizeResources(m_width, m_height);
}

RenderTexture::~RenderTexture()
{
    m_renderTargetView->Release();
    m_depthStencilView->Release();
    m_shaderResourceView->Release();
    m_renderTarget->Release();
}

void RenderTexture::resizeResources(size_t width, size_t height)
{
    ID3D11Device* device = GraphicsEngine::getInstance()->getRenderSystem()->getDirectXDevice();
    if (width == this->m_width && height == this->m_height || width == 0 || height == 0)
        return;

    if (this->m_width > UINT32_MAX || this->m_height > UINT32_MAX)
    {
        Logger::out_of_range("Invalid m_width/m_height");
    }

    if (!device)
        return;

    CD3D11_TEXTURE2D_DESC renderTargetDesc = {};
    renderTargetDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    renderTargetDesc.Width = static_cast<UINT>(width);
    renderTargetDesc.Height = static_cast<UINT>(height);
    renderTargetDesc.ArraySize = 1; // The render target view has only one texture.
    renderTargetDesc.MipLevels = 1; // Use a single mipmap level.
    renderTargetDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    renderTargetDesc.Usage = D3D11_USAGE_DEFAULT;
    renderTargetDesc.CPUAccessFlags = 0;
    renderTargetDesc.SampleDesc.Count = 1;
    
    if (!Logger::log(this, device->CreateTexture2D(&renderTargetDesc, nullptr, &this->m_renderTarget)))
        Logger::throw_exception("Texture not created successfully");

    if (!Logger::log(this, device->CreateRenderTargetView(m_renderTarget, NULL, &this->m_renderTargetView)))
        Logger::throw_exception("Render Target View not created successfully");

    if (!Logger::log(this, device->CreateShaderResourceView(m_renderTarget, NULL, &this->m_shaderResourceView)))
        Logger::throw_exception("Shader Resource View not created successfully");

    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    texDesc.MipLevels = 1;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.MiscFlags = 0;
    texDesc.ArraySize = 1;
    texDesc.CPUAccessFlags = 0;

    ID3D11Texture2D* buffer = NULL;

    if (!Logger::log(this, device->CreateTexture2D(&texDesc, NULL, &buffer)))
        Logger::throw_exception("Texture2D not created successfully");

    if (!Logger::log(this, device->CreateDepthStencilView(buffer, NULL, &m_depthStencilView)))
        Logger::throw_exception("Depth Stencil View not created successfully");

    this->m_width = width;
    this->m_height = height;
}

ID3D11Texture2D* RenderTexture::getRenderTarget()
{
    return this->m_renderTarget;
}

ID3D11RenderTargetView* RenderTexture::getRenderTargetView()
{
    return this->m_renderTargetView;
}

ID3D11ShaderResourceView* RenderTexture::getShaderResourceView()
{
    return this->m_shaderResourceView;
}
