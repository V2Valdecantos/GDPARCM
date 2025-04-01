#include "Sphere.h"
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

Sphere::Sphere(std::string name) : AGameObject(name)
{
	// Initialize Default Values
	this->rings = 32;
	this->sectors = 32;
	this->radius = 0.5f;
	this->height = 1.0f;

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	this->texture = TextureLibrary::getInstance()->getTexture(TextureName::DEFAULT);
	// 1. Create Indices and Index Buffer.
	std::vector<unsigned int> indices;

	// Precomputed values to optimize calculations
	float ringStep = 1.0f / (float)(rings - 1);    // Step size for the rings
	float sectorStep = 1.0f / (float)(sectors - 1); // Step size for the sectors
	float half = (float)rings / 2;

	int index = 0;
	for (unsigned int ring = 0; ring < rings - 1; ++ring)
	{
		for (unsigned int sector = 0; sector < sectors - 1; ++sector)
		{
			// Define two triangles for each quad on the sphere's surface
			// First triangle
			indices.push_back(ring * sectors + sector);
			indices.push_back((ring + 1) * sectors + sector);
			indices.push_back((ring + 1) * sectors + (sector + 1));

			// Second triangle
			indices.push_back(ring * sectors + sector);
			indices.push_back((ring + 1) * sectors + (sector + 1));
			indices.push_back(ring * sectors + (sector + 1));
		}
	}
	this->m_indexBuffer = renderSystem->createIndexBuffer(indices.data(), indices.size());

	// 2. Create vertices and vertex buffer.
	ShaderNames shaderNames; void* shaderByteCode = nullptr; size_t sizeShader = 0;

	std::vector<vertex> vertices;

	Vector2D uvs[] = { Vector2D(0, 0), Vector2D(0, 1), Vector2D(1, 0), Vector2D(1, 1) };

	for (int ring = 0; ring < rings; ++ring)
	{
		for (unsigned int sector = 0; sector < sectors; ++sector)
		{
			// Calculate the spherical coordinates for the vertex
			float y = sinf(-DirectX::XM_PIDIV2 + DirectX::XM_PI * ring * ringStep); // Vertical (Y) position
			float x = cosf(2 * DirectX::XM_PI * sector * sectorStep) * sinf(DirectX::XM_PI * ring * ringStep); // Horizontal (X) position
			float z = sinf(2 * DirectX::XM_PI * sector * sectorStep) * sinf(DirectX::XM_PI * ring * ringStep); // Depth (Z) position

			Vector2D uv = Vector2D(sector * sectorStep, ring * ringStep);

			// Create the vertex with position, normal, and texture coordinate
			vertex vert = {
				Vector3D(x * radius, y * radius, z * radius),
				uv
			};

			// Store the vertex in the vertex buffer
			vertices.push_back(vert);
		}
	}
	ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.TEXTURED_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);
	this->m_vertexBuffer = renderSystem->createTexturedVertexBuffer(vertices.data(), sizeof(vertex), vertices.size(), shaderByteCode, sizeShader);
	renderSystem->releaseCompiledShader();

	CBObjectData cbData;
	cbData.time = 0.0f;

	m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));
}

GDEngine::Sphere::Sphere(std::string guid, std::string name) : AGameObject(guid, name)
{
	// Initialize Default Values
	this->rings = 32;
	this->sectors = 32;
	this->radius = 0.5f;
	this->height = 1.0f;

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	this->texture = TextureLibrary::getInstance()->getTexture(TextureName::DEFAULT);
	// 1. Create Indices and Index Buffer.
	std::vector<unsigned int> indices;

	// Precomputed values to optimize calculations
	float ringStep = 1.0f / (float)(rings - 1);    // Step size for the rings
	float sectorStep = 1.0f / (float)(sectors - 1); // Step size for the sectors
	float half = (float)rings / 2;

	int index = 0;
	for (unsigned int ring = 0; ring < rings - 1; ++ring)
	{
		for (unsigned int sector = 0; sector < sectors - 1; ++sector)
		{
			// Define two triangles for each quad on the sphere's surface
			// First triangle
			indices.push_back(ring * sectors + sector);
			indices.push_back((ring + 1) * sectors + sector);
			indices.push_back((ring + 1) * sectors + (sector + 1));

			// Second triangle
			indices.push_back(ring * sectors + sector);
			indices.push_back((ring + 1) * sectors + (sector + 1));
			indices.push_back(ring * sectors + (sector + 1));
		}
	}
	this->m_indexBuffer = renderSystem->createIndexBuffer(indices.data(), indices.size());

	// 2. Create vertices and vertex buffer.
	ShaderNames shaderNames; void* shaderByteCode = nullptr; size_t sizeShader = 0;

	std::vector<vertex> vertices;

	Vector2D uvs[] = { Vector2D(0, 0), Vector2D(0, 1), Vector2D(1, 0), Vector2D(1, 1) };

	for (int ring = 0; ring < rings; ++ring)
	{
		for (unsigned int sector = 0; sector < sectors; ++sector)
		{
			// Calculate the spherical coordinates for the vertex
			float y = sinf(-DirectX::XM_PIDIV2 + DirectX::XM_PI * ring * ringStep); // Vertical (Y) position
			float x = cosf(2 * DirectX::XM_PI * sector * sectorStep) * sinf(DirectX::XM_PI * ring * ringStep); // Horizontal (X) position
			float z = sinf(2 * DirectX::XM_PI * sector * sectorStep) * sinf(DirectX::XM_PI * ring * ringStep); // Depth (Z) position

			Vector2D uv = Vector2D(sector * sectorStep, ring * ringStep);

			// Create the vertex with position, normal, and texture coordinate
			vertex vert = {
				Vector3D(x * radius, y * radius, z * radius),
				uv
			};

			// Store the vertex in the vertex buffer
			vertices.push_back(vert);
		}
	}
	ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.TEXTURED_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);
	this->m_vertexBuffer = renderSystem->createTexturedVertexBuffer(vertices.data(), sizeof(vertex), vertices.size(), shaderByteCode, sizeShader);
	renderSystem->releaseCompiledShader();

	CBObjectData cbData;
	cbData.time = 0.0f;

	m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));
}

Sphere::~Sphere()
{
	delete m_vertexBuffer;
	delete m_indexBuffer;
	delete m_constantBuffer;
	AGameObject::~AGameObject();
}

void Sphere::onCreate()
{
	AGameObject::onCreate();
}

void Sphere::update(float deltaTime)
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
void Sphere::draw(int height, int width)
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

void Sphere::onDestroy()
{

}
