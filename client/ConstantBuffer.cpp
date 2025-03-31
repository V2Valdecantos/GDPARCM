#include "ConstantBuffer.h"

#include <exception>

#include "RenderSystem.h"
#include "DeviceContext.h"
#include "Logger.h"

using namespace GDEngine;

ConstantBuffer::ConstantBuffer(RenderSystem* system, void* buffer, UINT size_buffer) : m_system(system)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_buffer;
	buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {  };
	init_data.pSysMem = buffer;

	if (!Logger::log(this, this->m_system->m_D3DDevice->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
		Logger::throw_exception("ConstantBuffer not created successfully");
}

ConstantBuffer::~ConstantBuffer()
{
	if (m_buffer)
		m_buffer->Release();
}

void ConstantBuffer::update(DeviceContext* context, void* buffer)
{
	context->m_deviceContext->UpdateSubresource(this->m_buffer, NULL, NULL, buffer, NULL, NULL);
}