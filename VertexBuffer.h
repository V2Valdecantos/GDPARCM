#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

namespace GDEngine
{
	class VertexBuffer
	{
	protected:
		RenderSystem* m_system = nullptr;

		UINT m_sizeVertex;
		UINT m_sizeList;

		ID3D11InputLayout* m_layout;
		ID3D11Buffer* m_buffer;

	protected:
		friend class DeviceContext;

	public:
		VertexBuffer(RenderSystem* system);
		virtual ~VertexBuffer();

	public:
		virtual void load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader);

	public:
		UINT getSizeVertexList();
	};
}