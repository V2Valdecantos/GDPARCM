#include "Texture.h"
#include <DirectXTex.h>

#include "GraphicsEngine.h"

#include "Logger.h"

using namespace GDEngine;

Texture::Texture(const wchar_t* fullPath) : Resource(fullPath)
{
	DirectX::ScratchImage imageData;
	HRESULT result = DirectX::LoadFromWICFile(fullPath, DirectX::WIC_FLAGS_NONE, nullptr, imageData);

	if (Logger::log(this, result))
	{
		ID3D11Device* device = GraphicsEngine::getInstance()->getRenderSystem()->getDirectXDevice();
		
		result = DirectX::CreateTexture(device, imageData.GetImages(), imageData.GetImageCount(), imageData.GetMetadata(), &m_texture);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = imageData.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = (UINT)imageData.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		device->CreateShaderResourceView(this->m_texture, &desc, &m_shaderResourceView);
	}
	else
	{
		Logger::throw_exception("Texture not created successfully");
	}
}

Texture::~Texture()
{
	m_shaderResourceView->Release();
	m_texture->Release();
}
