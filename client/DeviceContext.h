#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

namespace GDEngine
{
	class DeviceContext
	{
	private:
		RenderSystem* m_system = nullptr;
		ID3D11DeviceContext* m_deviceContext;

	private:
		friend class ConstantBuffer;

	public:
		DeviceContext(RenderSystem* system, ID3D11DeviceContext* deviceContext);
		~DeviceContext();

	public:
		ID3D11DeviceContext* getContext();

		//void clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha);
		void clearRenderTargetColor(const RenderTexture* renderTexture, float red, float green, float blue, float alpha);
		void setRenderTarget(const RenderTexture* renderTexture);
		void setVertexBuffer(const VertexBuffer* vertexBuffer);
		void setIndexBuffer(const IndexBuffer* indexBuffer);

		void drawTriangleList(UINT vertexCount, UINT startVertexIndex);
		void drawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation);
		void drawTriangleStrip(UINT vertexCount, UINT startVertexIndex);
		void drawLineStrip(UINT vertexCount, UINT startVertexIndex);

		void setViewportSize(UINT width, UINT height);
		void setViewport(const Viewport* vp);

		void setVertexShader(const VertexShader* vertexShader);
		void setPixelShader(const PixelShader* pixelShader);

		void setTexture(const Texture* texture, int index = 0);

		void setConstantBuffer(const ConstantBuffer* buffer, int index = 0);
		void setRasterizerState(const RasterizerState* rasterizerState);
	};
}

