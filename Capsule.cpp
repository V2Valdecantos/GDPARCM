#include "Capsule.h"
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

Capsule::Capsule(std::string name) : AGameObject(name)
{
	// Initialize Default Values
	this->stacks = 32;
	this->slices = 32;
	this->radius = 0.5f;
	this->height = 1.0f;

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	this->texture = TextureLibrary::getInstance()->getTexture(TextureName::DEFAULT);
	// 1. Create Indices and Index Buffer.
	std::vector<unsigned int> indices;
	
	// Precomputed values to optimize calculations
	float halfHeight = height / 2.0f;
    float sliceStep = 2 * DirectX::XM_PI / static_cast<float>(slices);
    float stackStep = DirectX::XM_PI / 2.0f / static_cast<float>(stacks); // Dividing a hemisphere

	// 2. Create vertices and vertex buffer.
	ShaderNames shaderNames; void* shaderByteCode = nullptr; size_t sizeShader = 0;

	std::vector<vertex> vertices;
	
	// Generate vertices for the top hemisphere
	for (unsigned int i = 0; i <= stacks; ++i)
	{
		float phi = i * stackStep; // Angle from the top of the hemisphere (0) to the equator (π/2)
		for (unsigned int j = 0; j <= slices; ++j)
		{
			float theta = j * sliceStep; // Angle around the circle (longitude)
			float x = radius * sinf(phi) * cosf(theta);
			float y = radius * cosf(phi);
			float z = radius * sinf(phi) * sinf(theta);

			float u = 2.0f * ((float)j / slices);       // U wraps around the longitude
			float v = (0.5f * (float)i / stacks); // 0 to 0.5

			// Position of vertex on the top hemisphere
			vertex vert = {
				Vector3D(x, y + halfHeight, z),
				Vector2D(u, v)
			};
			vertices.push_back(vert);
		}
	}
	// Generate vertices for the cylindrical body
	unsigned int cylinderStartIndex = vertices.size();
	for (unsigned int i = 0; i <= slices; ++i)
	{
		float theta = i * sliceStep;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);

		float u = 2.0f * ((float)i / slices); // U = 0 for the last slice to match the first slice
		float vTop = 0.5f;              // V flipped at the top
		float vBottom = 1.5f;			// V flipped at the bottom

		// Top vertex of the cylinder
		vertices.push_back({ Vector3D(x, halfHeight, z), Vector2D(u, vTop) });
		// Bottom vertex of the cylinder
		vertices.push_back({ Vector3D(x, -halfHeight, z), Vector2D(u, vBottom) });
	}

	// Generate vertices for the bottom hemisphere
	unsigned int bottomHemisphereStartIndex = vertices.size();
	for (unsigned int i = 0; i <= stacks; ++i)
	{
		float phi = i * stackStep; // Angle from the equator (π/2) to the bottom of the hemisphere (π)
		for (unsigned int j = 0; j <= slices; ++j)
		{
			float theta = j * sliceStep; // Angle around the circle (longitude)
			float x = radius * sinf(phi) * cosf(theta);
			float y = -radius * cosf(phi);
			float z = radius * sinf(phi) * sinf(theta);

			float u = 2.0f * ((float)j / slices);       // U wraps around the longitude
			float v = 2.0f - (0.5f * (float)i / stacks); // V goes from 2.0 to 1.5 as i goes from 0 to stacks

			// Position of vertex on the bottom hemisphere
			vertex vert = {
				Vector3D(x, y - halfHeight, z),
				Vector2D(u, v)
			};
			vertices.push_back(vert);
		}
	}

	// Generate indices for the top hemisphere
	for (unsigned int i = 0; i < stacks; ++i)
	{
		for (unsigned int j = 0; j < slices; ++j)
		{
			unsigned int current = i * (slices + 1) + j;
			unsigned int next = i * (slices + 1) + (j + 1);
			unsigned int belowCurrent = +(i + 1) * (slices + 1) + j;
			unsigned int belowNext = +(i + 1) * (slices + 1) + (j + 1);

			indices.push_back(current);
			indices.push_back(next);
			indices.push_back(belowCurrent);

			indices.push_back(next);
			indices.push_back(belowNext);
			indices.push_back(belowCurrent);
		}
	}
	// Generate indices for the cylindrical body
	for (unsigned int i = 0; i < slices; ++i)
	{
		unsigned int topIndex = cylinderStartIndex + i * 2;
		unsigned int bottomIndex = topIndex + 1;
		unsigned int nextTopIndex = cylinderStartIndex + ((i + 1) % slices) * 2;
		unsigned int nextBottomIndex = nextTopIndex + 1;

		// First triangle for the side
		indices.push_back(topIndex);
		indices.push_back(nextTopIndex);
		indices.push_back(bottomIndex);

		// Second triangle for the side
		indices.push_back(bottomIndex);
		indices.push_back(nextTopIndex);
		indices.push_back(nextBottomIndex);
	}
	// Generate indices for the bottom hemisphere
	for (unsigned int i = 0; i < stacks; ++i)
	{
		for (unsigned int j = 0; j < slices; ++j)
		{
			unsigned int current = bottomHemisphereStartIndex + i * (slices + 1) + j;
			unsigned int next = bottomHemisphereStartIndex + i * (slices + 1) + (j + 1);
			unsigned int belowCurrent = bottomHemisphereStartIndex + (i + 1) * (slices + 1) + j;
			unsigned int belowNext = bottomHemisphereStartIndex + (i + 1) * (slices + 1) + (j + 1);

			indices.push_back(current);
			indices.push_back(belowCurrent);
			indices.push_back(next);

			indices.push_back(next);
			indices.push_back(belowCurrent);
			indices.push_back(belowNext);
		}
	}

	this->m_indexBuffer = renderSystem->createIndexBuffer(indices.data(), indices.size());

	ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.TEXTURED_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);
	this->m_vertexBuffer = renderSystem->createTexturedVertexBuffer(vertices.data(), sizeof(vertex), vertices.size(), shaderByteCode, sizeShader);
	renderSystem->releaseCompiledShader();

	CBObjectData cbData;
	cbData.time = 0.0f;

	m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));
}

GDEngine::Capsule::Capsule(std::string guid, std::string name) : AGameObject(guid, name)
{
	// Initialize Default Values
	this->stacks = 32;
	this->slices = 32;
	this->radius = 0.5f;
	this->height = 1.0f;

	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	this->texture = TextureLibrary::getInstance()->getTexture(TextureName::DEFAULT);
	// 1. Create Indices and Index Buffer.
	std::vector<unsigned int> indices;

	// Precomputed values to optimize calculations
	float halfHeight = height / 2.0f;
	float sliceStep = 2 * DirectX::XM_PI / static_cast<float>(slices);
	float stackStep = DirectX::XM_PI / 2.0f / static_cast<float>(stacks); // Dividing a hemisphere

	// 2. Create vertices and vertex buffer.
	ShaderNames shaderNames; void* shaderByteCode = nullptr; size_t sizeShader = 0;

	std::vector<vertex> vertices;

	// Generate vertices for the top hemisphere
	for (unsigned int i = 0; i <= stacks; ++i)
	{
		float phi = i * stackStep; // Angle from the top of the hemisphere (0) to the equator (π/2)
		for (unsigned int j = 0; j <= slices; ++j)
		{
			float theta = j * sliceStep; // Angle around the circle (longitude)
			float x = radius * sinf(phi) * cosf(theta);
			float y = radius * cosf(phi);
			float z = radius * sinf(phi) * sinf(theta);

			float u = 2.0f * ((float)j / slices);       // U wraps around the longitude
			float v = (0.5f * (float)i / stacks); // 0 to 0.5

			// Position of vertex on the top hemisphere
			vertex vert = {
				Vector3D(x, y + halfHeight, z),
				Vector2D(u, v)
			};
			vertices.push_back(vert);
		}
	}
	// Generate vertices for the cylindrical body
	unsigned int cylinderStartIndex = vertices.size();
	for (unsigned int i = 0; i <= slices; ++i)
	{
		float theta = i * sliceStep;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);

		float u = 2.0f * ((float)i / slices); // U = 0 for the last slice to match the first slice
		float vTop = 0.5f;              // V flipped at the top
		float vBottom = 1.5f;			// V flipped at the bottom

		// Top vertex of the cylinder
		vertices.push_back({ Vector3D(x, halfHeight, z), Vector2D(u, vTop) });
		// Bottom vertex of the cylinder
		vertices.push_back({ Vector3D(x, -halfHeight, z), Vector2D(u, vBottom) });
	}

	// Generate vertices for the bottom hemisphere
	unsigned int bottomHemisphereStartIndex = vertices.size();
	for (unsigned int i = 0; i <= stacks; ++i)
	{
		float phi = i * stackStep; // Angle from the equator (π/2) to the bottom of the hemisphere (π)
		for (unsigned int j = 0; j <= slices; ++j)
		{
			float theta = j * sliceStep; // Angle around the circle (longitude)
			float x = radius * sinf(phi) * cosf(theta);
			float y = -radius * cosf(phi);
			float z = radius * sinf(phi) * sinf(theta);

			float u = 2.0f * ((float)j / slices);       // U wraps around the longitude
			float v = 2.0f - (0.5f * (float)i / stacks); // V goes from 2.0 to 1.5 as i goes from 0 to stacks

			// Position of vertex on the bottom hemisphere
			vertex vert = {
				Vector3D(x, y - halfHeight, z),
				Vector2D(u, v)
			};
			vertices.push_back(vert);
		}
	}

	// Generate indices for the top hemisphere
	for (unsigned int i = 0; i < stacks; ++i)
	{
		for (unsigned int j = 0; j < slices; ++j)
		{
			unsigned int current = i * (slices + 1) + j;
			unsigned int next = i * (slices + 1) + (j + 1);
			unsigned int belowCurrent = +(i + 1) * (slices + 1) + j;
			unsigned int belowNext = +(i + 1) * (slices + 1) + (j + 1);

			indices.push_back(current);
			indices.push_back(next);
			indices.push_back(belowCurrent);

			indices.push_back(next);
			indices.push_back(belowNext);
			indices.push_back(belowCurrent);
		}
	}
	// Generate indices for the cylindrical body
	for (unsigned int i = 0; i < slices; ++i)
	{
		unsigned int topIndex = cylinderStartIndex + i * 2;
		unsigned int bottomIndex = topIndex + 1;
		unsigned int nextTopIndex = cylinderStartIndex + ((i + 1) % slices) * 2;
		unsigned int nextBottomIndex = nextTopIndex + 1;

		// First triangle for the side
		indices.push_back(topIndex);
		indices.push_back(nextTopIndex);
		indices.push_back(bottomIndex);

		// Second triangle for the side
		indices.push_back(bottomIndex);
		indices.push_back(nextTopIndex);
		indices.push_back(nextBottomIndex);
	}
	// Generate indices for the bottom hemisphere
	for (unsigned int i = 0; i < stacks; ++i)
	{
		for (unsigned int j = 0; j < slices; ++j)
		{
			unsigned int current = bottomHemisphereStartIndex + i * (slices + 1) + j;
			unsigned int next = bottomHemisphereStartIndex + i * (slices + 1) + (j + 1);
			unsigned int belowCurrent = bottomHemisphereStartIndex + (i + 1) * (slices + 1) + j;
			unsigned int belowNext = bottomHemisphereStartIndex + (i + 1) * (slices + 1) + (j + 1);

			indices.push_back(current);
			indices.push_back(belowCurrent);
			indices.push_back(next);

			indices.push_back(next);
			indices.push_back(belowCurrent);
			indices.push_back(belowNext);
		}
	}

	this->m_indexBuffer = renderSystem->createIndexBuffer(indices.data(), indices.size());

	ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.TEXTURED_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);
	this->m_vertexBuffer = renderSystem->createTexturedVertexBuffer(vertices.data(), sizeof(vertex), vertices.size(), shaderByteCode, sizeShader);
	renderSystem->releaseCompiledShader();

	CBObjectData cbData;
	cbData.time = 0.0f;

	m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));
}

Capsule::~Capsule()
{
	delete m_vertexBuffer;
	delete m_indexBuffer;
	delete m_constantBuffer;
	AGameObject::~AGameObject();
}

void Capsule::onCreate()
{
	AGameObject::onCreate();
}

void Capsule::update(float deltaTime)
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
void Capsule::draw(int height, int width)
{
	ShaderNames shaderNames;
	DeviceContext* deviceContext = GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext();
	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
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

void Capsule::onDestroy()
{

}