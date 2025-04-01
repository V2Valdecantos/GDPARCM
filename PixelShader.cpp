#include "PixelShader.h"

#include <exception>

#include "RenderSystem.h"
#include "Logger.h"

using namespace GDEngine;

PixelShader::PixelShader(RenderSystem* system, const void* shader_byte_code, size_t byte_code_size) : m_system(system)
{
	if (!Logger::log(this, this->m_system->m_D3DDevice->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &m_pixelShader)))
		Logger::throw_exception("PixelShader not created successfully");
}

PixelShader::~PixelShader()
{
	m_pixelShader->Release();
}