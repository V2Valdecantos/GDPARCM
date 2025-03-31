#pragma once

#include "Resource.h"
#include <d3d11.h>

namespace GDEngine
{
	class CreditsScreen;
}

namespace GDEngine
{
	class Texture : public Resource
	{
	private:
		ID3D11Resource* m_texture = nullptr;
		

		friend class GDEngine::CreditsScreen;
		friend class DeviceContext;
	public:
		ID3D11ShaderResourceView* m_shaderResourceView = nullptr;
		Texture(const wchar_t* fullPath);
		~Texture() override;
	};
}
