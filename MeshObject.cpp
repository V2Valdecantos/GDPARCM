#include "MeshObject.h"

#include "EngineTime.h"
#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "Renderer.h"
#include "ShaderLibrary.h"
#include "TextureLibrary.h"
#include "Mesh.h"

namespace GDEngine
{
	MeshObject::MeshObject(std::string name, const wchar_t* meshFilePath) : AGameObject(name)
	{
		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
		MeshManager* meshManager = GraphicsEngine::getInstance()->getMeshManager();

		this->m_mesh = meshManager->createMeshFromFile(meshFilePath);
		this->texture = TextureLibrary::getInstance()->getTexture(TextureName::DEFAULT);

		CBObjectData cbData;
		cbData.time = 0.0f;

		m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));
	}

	MeshObject::MeshObject(std::string guid, std::string name, const wchar_t* meshFilePath) : AGameObject(guid, name)
	{
		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
		MeshManager* meshManager = GraphicsEngine::getInstance()->getMeshManager();

		this->m_mesh = meshManager->createMeshFromFile(meshFilePath);
		this->texture = TextureLibrary::getInstance()->getTexture(TextureName::DEFAULT);

		CBObjectData cbData;
		cbData.time = 0.0f;

		m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));
	}

	MeshObject::~MeshObject()
	{
		delete this->m_constantBuffer;
	}

	void MeshObject::onCreate()
	{
		AGameObject::onCreate();
	}

	void MeshObject::update(float deltaTime)
	{

		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
		CBObjectData cbObjectData;

		cbObjectData.time = 0.0f;

		this->updateLocalMatrix();

		cbObjectData.worldMatrix.setMatrix(this->m_localMatrix);

		m_constantBuffer->update(renderSystem->getImmediateDeviceContext(), &cbObjectData);

		AGameObject::update(deltaTime);
	}

	void MeshObject::draw(int width, int height)
	{
		ShaderNames shaderNames;
		DeviceContext* deviceContext = GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext();

		VertexShader* vertexShader = ShaderLibrary::getInstance()->getVertexShader(shaderNames.TEXTURED_VERTEX_SHADER_NAME);
		PixelShader* pixelShader = ShaderLibrary::getInstance()->getPixelShader(shaderNames.TEXTURED_PIXEL_SHADER_NAME);

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

		deviceContext->setConstantBuffer(m_constantBuffer, 0);

		deviceContext->setVertexBuffer(this->m_mesh->getVertexBuffer());
		deviceContext->setIndexBuffer(this->m_mesh->getIndexBuffer());

		deviceContext->drawIndexedTriangleList(this->m_mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);

		AGameObject::draw(width, height);
	}

	void MeshObject::onDestroy()
	{
		AGameObject::onDestroy();
	}
}
