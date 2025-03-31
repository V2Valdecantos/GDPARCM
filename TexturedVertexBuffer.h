#pragma once

#include <d3d11.h>
#include "Prerequisites.h"
#include "VertexBuffer.h"

namespace GDEngine
{
	class TexturedVertexBuffer : public VertexBuffer
	{
	public:
		TexturedVertexBuffer(RenderSystem* system);
		~TexturedVertexBuffer() override;

	public:
		void load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader) override;
	};
}