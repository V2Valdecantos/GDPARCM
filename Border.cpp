#include "Border.h"

#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "Camera.h"

#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "ShaderLibrary.h"

using namespace GDEngine;

Border::Border(std::string name) : AGameObject(name)
{
	vertex list[] =
	{
		{ Vector3D(-1.0f,-1.0f,0.0f),   Vector3D(0,0,0), Vector3D(0,0,0) },
		{ Vector3D(-1.0f,1.0f,0.0f),   Vector3D(0,0,0), Vector3D(0,0,0) },
		{ Vector3D(1.0f,1.0f,0.0f),Vector3D(0,0,0),  Vector3D(0,0,0) },
		{ Vector3D(1.0f,-1.0f,0.0f),    Vector3D(0,0,0), Vector3D(0,0,0) },
		{ Vector3D(-1.0f,-1.0f,0.0f),    Vector3D(0,0,0), Vector3D(0,0,0) }
	};

	CBObjectData cbData;
	cbData.time = 0.0f;

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	this->m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));

	UINT sizeList = ARRAYSIZE(list);

	ShaderNames shaderNames;

	void* shaderByteCode = NULL;
	size_t sizeShader = 0;

	ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.BASE_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);

	this->m_vertexBuffer = renderSystem->createVertexBuffer(list, sizeof(vertex), sizeList, shaderByteCode, sizeShader);
}

Border::~Border()
{
}

void Border::onCreate()
{
	AGameObject::onCreate();
}

void Border::update(float deltaTime)
{
	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	CBObjectData cbData;
	cbData.time = 0.0f;

	cbData.worldMatrix.setIdentity();

	this->m_constantBuffer->update(renderSystem->getImmediateDeviceContext(), &cbData);
}

// Sets shaders and draws afterwards
void Border::draw(int width, int height)
{
	ShaderNames shaderNames;

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	VertexShader* vertexShader = ShaderLibrary::getInstance()->getVertexShader(shaderNames.BASE_VERTEX_SHADER_NAME);
	PixelShader* pixelShader = ShaderLibrary::getInstance()->getPixelShader(shaderNames.BASE_PIXEL_SHADER_NAME);

	renderSystem->getImmediateDeviceContext()->setConstantBuffer(m_constantBuffer, 0);

	renderSystem->getImmediateDeviceContext()->setVertexShader(vertexShader);
	renderSystem->getImmediateDeviceContext()->setPixelShader(pixelShader);

	renderSystem->getImmediateDeviceContext()->setVertexBuffer(this->m_vertexBuffer);

	renderSystem->getImmediateDeviceContext()->drawLineStrip(this->m_vertexBuffer->getSizeVertexList(), 0);
}

void Border::onDestroy()
{
	delete this->m_constantBuffer;
	delete this->m_vertexBuffer;
}
