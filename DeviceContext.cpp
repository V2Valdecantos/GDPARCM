#include "DeviceContext.h"

#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "Viewport.h"
#include "RenderTexture.h"
#include "Texture.h"


using namespace GDEngine;

DeviceContext::DeviceContext(RenderSystem* system, ID3D11DeviceContext* deviceContext) : m_system(system), m_deviceContext(deviceContext)
{

}

DeviceContext::~DeviceContext()
{
	m_deviceContext->Release();
}

ID3D11DeviceContext* DeviceContext::getContext()
{
	return this->m_deviceContext;
}

void DeviceContext::clearRenderTargetColor(const RenderTexture* renderTexture, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red, green, blue, alpha };
	m_deviceContext->ClearRenderTargetView(renderTexture->m_renderTargetView, clear_color);
	m_deviceContext->ClearDepthStencilView(renderTexture->m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_deviceContext->OMSetRenderTargets(1, &renderTexture->m_renderTargetView, renderTexture->m_depthStencilView);
}

void DeviceContext::setRenderTarget(const RenderTexture* renderTexture)
{
	m_deviceContext->OMSetRenderTargets(1, &renderTexture->m_renderTargetView, renderTexture->m_depthStencilView);
}

void DeviceContext::setVertexBuffer(const VertexBuffer* vertexBuffer)
{
	UINT stride = vertexBuffer->m_sizeVertex;
	UINT offset = 0;

	m_deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer->m_buffer, &stride, &offset);
	m_deviceContext->IASetInputLayout(vertexBuffer->m_layout);
}

void DeviceContext::setIndexBuffer(const IndexBuffer* indexBuffer)
{
	m_deviceContext->IASetIndexBuffer(indexBuffer->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::drawTriangleList(UINT vertexCount, UINT startVertexIndex)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->Draw(vertexCount, startVertexIndex);
}

void DeviceContext::drawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->DrawIndexed(indexCount, startIndexLocation, startVertexIndex);
}

void DeviceContext::drawTriangleStrip(UINT vertexCount, UINT startVertexIndex)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_deviceContext->Draw(vertexCount, startVertexIndex);
}

void DeviceContext::drawLineStrip(UINT vertexCount, UINT startVertexIndex)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	m_deviceContext->Draw(vertexCount, startVertexIndex);
}

void DeviceContext::setViewport(const Viewport* vp)
{
	m_deviceContext->RSSetViewports(1, &vp->m_viewport);
}

void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1, &vp);
}

void DeviceContext::setVertexShader(const VertexShader* vertexShader)
{
	m_deviceContext->VSSetShader(vertexShader->m_vertexShader, nullptr, 0);
}

void DeviceContext::setPixelShader(const PixelShader* pixelShader)
{
	m_deviceContext->PSSetShader(pixelShader->m_pixelShader, nullptr, 0);
}

void DeviceContext::setTexture(const Texture* texture, int index)
{
	m_deviceContext->VSSetShaderResources(index, 1, &texture->m_shaderResourceView);
	m_deviceContext->PSSetShaderResources(index, 1, &texture->m_shaderResourceView);
}

void ::DeviceContext::setConstantBuffer(const ConstantBuffer* buffer, int index)
{
	m_deviceContext->VSSetConstantBuffers(index, 1, &buffer->m_buffer);
	m_deviceContext->PSSetConstantBuffers(index, 1, &buffer->m_buffer);
}

void DeviceContext::setRasterizerState(const RasterizerState* rasterizerState)
{
	m_deviceContext->RSSetState(rasterizerState->m_rasterizerState);
}