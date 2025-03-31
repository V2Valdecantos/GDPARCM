#include "RasterizerState.h"

#include <exception>

#include "RenderSystem.h"
#include "Logger.h"

using namespace GDEngine;

RasterizerState::RasterizerState(RenderSystem* system, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode) : m_system(system)
{
	D3D11_RASTERIZER_DESC desc = {};
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = fillMode;
	desc.CullMode = cullMode;

	if (!Logger::log(this, this->m_system->m_D3DDevice->CreateRasterizerState(&desc, &m_rasterizerState)))
		Logger::throw_exception("RasterizerState not created successfully");
}

RasterizerState::~RasterizerState()
{
	m_rasterizerState->Release();
}
