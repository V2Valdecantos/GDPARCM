#include "MeshObject.h"

#include "GraphicsEngine.h"

#include "Mesh.h"

namespace GDEngine
{
	MeshObject::MeshObject(std::string name, const wchar_t* meshFilePath) : AGameObject(name)
	{
		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
		MeshManager* meshManager = GraphicsEngine::getInstance()->getMeshManager();

		this->m_mesh = meshManager->createMeshFromFile(meshFilePath);

		CBObjectData cbData;
		cbData.time = 0.0f;

		m_constantBuffer = renderSystem->createConstantBuffer(&cbData, sizeof(CBObjectData));
	}

	MeshObject::MeshObject(std::string guid, std::string name, const wchar_t* meshFilePath) : AGameObject(guid, name)
	{
		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
		MeshManager* meshManager = GraphicsEngine::getInstance()->getMeshManager();

		this->m_mesh = meshManager->createMeshFromFile(meshFilePath);

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
		AGameObject::update(deltaTime);
	}

	void MeshObject::draw(int width, int height)
	{
		DeviceContext* deviceContext = GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext();

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
