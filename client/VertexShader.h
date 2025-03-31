#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

namespace GDEngine
{
	class VertexShader
	{
	private:
		RenderSystem* m_system = nullptr;
		ID3D11VertexShader* m_vertexShader;

	private:
		friend class RenderSystem;
		friend class DeviceContext;

	public:
		VertexShader(RenderSystem* system, const void* shaderByteCode, size_t byteCodeSize);
		~VertexShader();
	};
}
