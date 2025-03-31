#include "Cylinder.h"
#include <DirectXMath.h>

#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "CameraManager.h"
#include "EngineTime.h"
#include "ShaderLibrary.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "Renderer.h"

using namespace GDEngine;

Cylinder::Cylinder(std::string name) : AGameObject(name)
{
	// Initialize Default Values
	this->height = 2.0f;
	this->slices = 32.0f;
	this->radius = 1.0f;
	this->stacks = 1.0f;

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
	this->texture = TextureLibrary::getInstance()->getTexture(TextureName::DEFAULT);

	// 1. Create Indices and Index Buffer.
	std::vector<unsigned int> indices;

	// Generate indices for the top cap
	int topCenterIndex = 0;
	for (int i = 1; i <= slices; ++i)
	{
		int nextIndex = (i % slices) + 1;
		indices.push_back(topCenterIndex);  // Center of the top cap
		indices.push_back(nextIndex);       // Next vertex on the circle
		indices.push_back(i);               // Current vertex on the circle
	}

	// Generate indices for the bottom cap
	unsigned int bottomCenterIndex = slices + 1;
	for (unsigned int i = 0; i < slices; ++i)
	{
		unsigned int currentVertexIndex = bottomCenterIndex + 1 + i;
		unsigned int nextVertexIndex = bottomCenterIndex + 1 + ((i + 1) % slices);

		indices.push_back(bottomCenterIndex);      // Center of the bottom cap
		indices.push_back(currentVertexIndex);     // Current vertex on the circle
		indices.push_back(nextVertexIndex);        // Next vertex on the circle
	}

	// Generate indices for the side walls
	int sideStartIndex = bottomCenterIndex + slices + 1;
	for (int i = 0; i < slices; ++i)
	{
		int topIndex = sideStartIndex + i * 2;
		int bottomIndex = topIndex + 1;
		int nextTopIndex = sideStartIndex + ((i + 1) % slices) * 2;
		int nextBottomIndex = nextTopIndex + 1;

		// First triangle for the side
		indices.push_back(topIndex);
		indices.push_back(nextTopIndex);
		indices.push_back(bottomIndex);

		// Second triangle for the side
		indices.push_back(bottomIndex);
		indices.push_back(nextTopIndex);
		indices.push_back(nextBottomIndex);
	}
	this->m_indexBuffer = renderSystem->createIndexBuffer(indices.data(), indices.size());

	// 2. Create vertices and vertex buffer.
	ShaderNames shaderNames; void* shaderByteCode = nullptr; size_t sizeShader = 0;

	std::vector<vertex> vertices;

	float halfHeight = height / 2.0f;
	float sliceStep = 2 * DirectX::XM_PI / static_cast<float>(slices);

	// Generate vertices for the top cap
	vertex topCenter = { Vector3D(0.0f, halfHeight, 0.0f), Vector2D(0.5f, 0.5f) };
	//Vertex3D topCenter = { Vector3(0.0f, halfHeight, 0.0f), Vector3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f) };
	vertices.push_back(topCenter);

	for (unsigned int i = 0; i < slices; ++i)
	{
		float theta = i * sliceStep;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		Vector2D uv = Vector2D((x / radius + 1) * 0.5f, (z / radius + 1) * 0.5f);
		vertex topVertex = { Vector3D(x, halfHeight, z), uv };
		//Vertex3D topVertex = { Vector3(x, halfHeight, z), Vector3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2((x / radius + 1) * 0.5f, (z / radius + 1) * 0.5f) };
		vertices.push_back(topVertex);
	}

	// Generate vertices for the bottom cap
	vertex bottomCenter = { Vector3D(0.0f, -halfHeight, 0.0f), Vector2D(0.5f, 0.5f) };
	//Vertex bottomCenter = { Vector3(0.0f, -halfHeight, 0.0f), Vector3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f) };
	vertices.push_back(bottomCenter);

	for (unsigned int i = 0; i < slices; ++i)
	{
		float theta = i * sliceStep;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		Vector2D uv = Vector2D((x / radius + 1) * 0.5f, (z / radius + 1) * 0.5f);
		vertex bottomVertex = { Vector3D(x, -halfHeight, z), uv };
		vertices.push_back(bottomVertex);
	}

	// Generate vertices for the side wall
	for (unsigned int i = 0; i < slices; ++i)
	{
		float theta = i * sliceStep;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);

		// Top vertex of the side
		vertex sideTopVertex = { Vector3D(x, halfHeight, z), Vector2D(i / static_cast<float>(slices), 0.0f) };
		vertices.push_back(sideTopVertex);

		// Bottom vertex of the side
		vertex sideBottomVertex = { Vector3D(x, -halfHeight, z), Vector2D(i / static_cast<float>(slices), 1.0f) };
		vertices.push_back(sideBottomVertex);
	}

	ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.TEXTURED_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);
	this->m_vertexBuffer = renderSystem->createTexturedVertexBuffer(vertices.data(), sizeof(vertex), vertices.size(), shaderByteCode, sizeShader);
	renderSystem->releaseCompiledShader();

	CBObjectData cbData;
	cbData.time = 0.0f;

	m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));
}

GDEngine::Cylinder::Cylinder(std::string guid, std::string name) : AGameObject(guid, name)
{
	// Initialize Default Values
	this->height = 2.0f;
	this->slices = 32.0f;
	this->radius = 1.0f;
	this->stacks = 1.0f;

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
	this->texture = TextureLibrary::getInstance()->getTexture(TextureName::DEFAULT);

	// 1. Create Indices and Index Buffer.
	std::vector<unsigned int> indices;

	// Generate indices for the top cap
	int topCenterIndex = 0;
	for (int i = 1; i <= slices; ++i)
	{
		int nextIndex = (i % slices) + 1;
		indices.push_back(topCenterIndex);  // Center of the top cap
		indices.push_back(nextIndex);       // Next vertex on the circle
		indices.push_back(i);               // Current vertex on the circle
	}

	// Generate indices for the bottom cap
	unsigned int bottomCenterIndex = slices + 1;
	for (unsigned int i = 0; i < slices; ++i)
	{
		unsigned int currentVertexIndex = bottomCenterIndex + 1 + i;
		unsigned int nextVertexIndex = bottomCenterIndex + 1 + ((i + 1) % slices);

		indices.push_back(bottomCenterIndex);      // Center of the bottom cap
		indices.push_back(currentVertexIndex);     // Current vertex on the circle
		indices.push_back(nextVertexIndex);        // Next vertex on the circle
	}

	// Generate indices for the side walls
	int sideStartIndex = bottomCenterIndex + slices + 1;
	for (int i = 0; i < slices; ++i)
	{
		int topIndex = sideStartIndex + i * 2;
		int bottomIndex = topIndex + 1;
		int nextTopIndex = sideStartIndex + ((i + 1) % slices) * 2;
		int nextBottomIndex = nextTopIndex + 1;

		// First triangle for the side
		indices.push_back(topIndex);
		indices.push_back(nextTopIndex);
		indices.push_back(bottomIndex);

		// Second triangle for the side
		indices.push_back(bottomIndex);
		indices.push_back(nextTopIndex);
		indices.push_back(nextBottomIndex);
	}
	this->m_indexBuffer = renderSystem->createIndexBuffer(indices.data(), indices.size());

	// 2. Create vertices and vertex buffer.
	ShaderNames shaderNames; void* shaderByteCode = nullptr; size_t sizeShader = 0;

	std::vector<vertex> vertices;

	float halfHeight = height / 2.0f;
	float sliceStep = 2 * DirectX::XM_PI / static_cast<float>(slices);

	// Generate vertices for the top cap
	vertex topCenter = { Vector3D(0.0f, halfHeight, 0.0f), Vector2D(0.5f, 0.5f) };
	//Vertex3D topCenter = { Vector3(0.0f, halfHeight, 0.0f), Vector3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f) };
	vertices.push_back(topCenter);

	for (unsigned int i = 0; i < slices; ++i)
	{
		float theta = i * sliceStep;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		Vector2D uv = Vector2D((x / radius + 1) * 0.5f, (z / radius + 1) * 0.5f);
		vertex topVertex = { Vector3D(x, halfHeight, z), uv };
		//Vertex3D topVertex = { Vector3(x, halfHeight, z), Vector3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2((x / radius + 1) * 0.5f, (z / radius + 1) * 0.5f) };
		vertices.push_back(topVertex);
	}

	// Generate vertices for the bottom cap
	vertex bottomCenter = { Vector3D(0.0f, -halfHeight, 0.0f), Vector2D(0.5f, 0.5f) };
	//Vertex bottomCenter = { Vector3(0.0f, -halfHeight, 0.0f), Vector3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f) };
	vertices.push_back(bottomCenter);

	for (unsigned int i = 0; i < slices; ++i)
	{
		float theta = i * sliceStep;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		Vector2D uv = Vector2D((x / radius + 1) * 0.5f, (z / radius + 1) * 0.5f);
		vertex bottomVertex = { Vector3D(x, -halfHeight, z), uv };
		vertices.push_back(bottomVertex);
	}

	// Generate vertices for the side wall
	for (unsigned int i = 0; i < slices; ++i)
	{
		float theta = i * sliceStep;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);

		// Top vertex of the side
		vertex sideTopVertex = { Vector3D(x, halfHeight, z), Vector2D(i / static_cast<float>(slices), 0.0f) };
		vertices.push_back(sideTopVertex);

		// Bottom vertex of the side
		vertex sideBottomVertex = { Vector3D(x, -halfHeight, z), Vector2D(i / static_cast<float>(slices), 1.0f) };
		vertices.push_back(sideBottomVertex);
	}

	ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.TEXTURED_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);
	this->m_vertexBuffer = renderSystem->createTexturedVertexBuffer(vertices.data(), sizeof(vertex), vertices.size(), shaderByteCode, sizeShader);
	renderSystem->releaseCompiledShader();

	CBObjectData cbData;
	cbData.time = 0.0f;

	m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));
}

Cylinder::~Cylinder()
{
	delete m_vertexBuffer;
	delete m_indexBuffer;
	delete m_constantBuffer;
	AGameObject::~AGameObject();
}

void Cylinder::onCreate()
{
	AGameObject::onCreate();
}

void Cylinder::update(float deltaTime)
{
	AGameObject::update(deltaTime);
	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
	CBObjectData cbObjectData;

	cbObjectData.time = 0.0f;

	this->updateLocalMatrix();

	cbObjectData.worldMatrix.setMatrix(this->m_localMatrix);

	m_constantBuffer->update(renderSystem->getImmediateDeviceContext(), &cbObjectData);
}

// Sets shaders and draws afterwards
void Cylinder::draw(int height, int width)
{
	ShaderNames shaderNames;
	DeviceContext* deviceContext = GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext();
	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
	Texture* texture = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(L"assets/textures/pxfuel.jpg");

	VertexShader* vertexShader = ShaderLibrary::getInstance()->getVertexShader(shaderNames.TEXTURED_VERTEX_SHADER_NAME);
	PixelShader* pixelShader = ShaderLibrary::getInstance()->getPixelShader(shaderNames.TEXTURED_PIXEL_SHADER_NAME);

	if (this->texture != nullptr)
		deviceContext->setTexture(this->texture);
	deviceContext->setConstantBuffer(m_constantBuffer, 0);

	std::vector<AComponent*> rendererList = this->getComponentsOfType(AComponent::Renderer);
	if (!rendererList.empty())
	{
		ARenderer* renderer;
		for (AComponent* component : rendererList) {
			renderer = (ARenderer*)component;
			renderer->perform(EngineTime::getDeltaTime());
		}

		rendererList = this->getComponentsOfTypeRecursive(AComponent::Renderer);

		for (AComponent* component : rendererList) {
			renderer = (ARenderer*)component;
			renderer->perform(EngineTime::getDeltaTime());
		}
		return;
	}

	deviceContext->setVertexShader(vertexShader);
	deviceContext->setPixelShader(pixelShader);

	deviceContext->setVertexBuffer(m_vertexBuffer);
	deviceContext->setIndexBuffer(m_indexBuffer);

	deviceContext->drawIndexedTriangleList(m_indexBuffer->getSizeIndexList(), 0, 0);
}

void Cylinder::onDestroy()
{

}
