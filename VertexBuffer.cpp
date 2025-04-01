#include "VertexBuffer.h"

#include <exception>

#include "RenderSystem.h"
#include "Logger.h"

using namespace GDEngine;

VertexBuffer::VertexBuffer(RenderSystem* system) : m_system(system), m_layout(0), m_buffer(0) {}

VertexBuffer::~VertexBuffer()
{
	m_layout->Release();
	m_buffer->Release();
}

void VertexBuffer::load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_vertex * size_list;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {  };
	init_data.pSysMem = list_vertices;

	m_sizeVertex = size_vertex;
	m_sizeList = size_list;

	if (!Logger::log(this, this->m_system->m_D3DDevice->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
		Logger::throw_exception("VertexBuffer not created successfully");

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		// SEMANTINC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGHNED BYT OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT size_layout = ARRAYSIZE(layout);

	if (!Logger::log(this, this->m_system->m_D3DDevice->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_layout)))
		Logger::throw_exception("InputLayout not created successfully");
}

UINT VertexBuffer::getSizeVertexList()
{
	return this->m_sizeList;
}
