#include "IndexBuffer.h"

#include <exception>

#include "RenderSystem.h"
#include "Logger.h"

using namespace GDEngine;

IndexBuffer::IndexBuffer(RenderSystem* system, void* list_indices, UINT size_list) : m_system(system), m_buffer(0)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = 4 * size_list;
	buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {  };
	init_data.pSysMem = list_indices;

	m_sizeList = size_list;

	if (!Logger::log(this, this->m_system->m_D3DDevice->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
		Logger::throw_exception("IndexBuffer not created successfully");
}

IndexBuffer::~IndexBuffer()
{
	m_buffer->Release();
}

UINT IndexBuffer::getSizeIndexList()
{
	return this->m_sizeList;
}