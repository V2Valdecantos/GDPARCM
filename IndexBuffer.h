#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

namespace GDEngine
{
	class IndexBuffer
	{
	private:
		RenderSystem* m_system = nullptr;

		UINT m_sizeList;
		ID3D11Buffer* m_buffer;

	private:
		friend class DeviceContext;

	public:
		IndexBuffer(RenderSystem* system, void* list_indices, UINT size_list);
		~IndexBuffer();

	public:
		UINT getSizeIndexList();
	};
}