#include "VertexShader.h"

#include <exception>

#include "RenderSystem.h"
#include "Logger.h"

using namespace GDEngine;

VertexShader::VertexShader(RenderSystem* system, const void* shaderByteCode, size_t byteCodeSize) : m_system(system)
{
	if (!Logger::log(this, this->m_system->m_D3DDevice->CreateVertexShader(shaderByteCode, byteCodeSize, nullptr, &m_vertexShader)))
		Logger::throw_exception("VertexShader not created successfully");
}

VertexShader::~VertexShader()
{
	m_vertexShader->Release();
}