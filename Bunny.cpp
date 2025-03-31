#include "Bunny.h"

#include "EngineTime.h"
#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "Renderer.h"
#include "ShaderLibrary.h"
#include "TextureLibrary.h"

namespace GDEngine {
	Bunny::Bunny(std::string name) : MeshObject(name,  L"assets/meshes/bunny.obj")
	{
		this->texture = TextureLibrary::getInstance()->getTexture(TextureName::DEFAULT);
		this->setScale(4, 4, 4);
	}

	Bunny::Bunny(std::string guid, std::string name) : MeshObject(guid, name,  L"assets/meshes/bunny.obj")
	{
		this->texture = TextureLibrary::getInstance()->getTexture(TextureName::DEFAULT);
		this->setScale(4, 4, 4);
	}

	Bunny::~Bunny()
	{
		MeshObject::~MeshObject();
	}

	void Bunny::onCreate()
	{
		AGameObject::onCreate();
	}

	void Bunny::update(float deltaTime)
	{
		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
		CBObjectData cbObjectData;

		cbObjectData.time = 0.0f;

		this->updateLocalMatrix();

		cbObjectData.worldMatrix.setMatrix(this->m_localMatrix);

		m_constantBuffer->update(renderSystem->getImmediateDeviceContext(), &cbObjectData);
	}

	void Bunny::draw(int width, int height)
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

		MeshObject::draw(width, height);
	}

	void Bunny::onDestroy()
	{
		AGameObject::onDestroy();
	}
}