#pragma once

#include <d3d11.h>

namespace GDEngine
{
    class RenderTexture
    {
    private:
        ID3D11Texture2D* m_renderTarget = nullptr;
        ID3D11RenderTargetView* m_renderTargetView = nullptr;
        ID3D11ShaderResourceView* m_shaderResourceView = nullptr;
        ID3D11DepthStencilView* m_depthStencilView = nullptr;

        size_t m_width;
        size_t m_height;

    private:
        friend class DeviceContext;
        friend class SwapChain;

    public:
        RenderTexture();
        ~RenderTexture();

        void resizeResources(size_t width, size_t height);

        ID3D11Texture2D* getRenderTarget();
        ID3D11RenderTargetView* getRenderTargetView();
        ID3D11ShaderResourceView* getShaderResourceView();
    };
}