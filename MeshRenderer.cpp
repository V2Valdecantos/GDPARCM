#include "MeshRenderer.h"

#include "GraphicsEngine.h"
#include "ConstantBuffer.h"
#include "Mesh.h"

namespace GDEngine
{
	MeshRenderer::MeshRenderer(std::string name, AGameObject* owner) : ARenderer(name, owner)
	{
	}

	MeshRenderer::MeshRenderer(std::string guid, std::string name, AGameObject* owner) : ARenderer(guid, name, owner)
	{

	}

	MeshRenderer::~MeshRenderer()
	{
	}

	void MeshRenderer::perform(float deltaTime)
	{
		if (m_mesh)
		{
			DeviceContext* deviceContext = GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext();

			deviceContext->setVertexBuffer(this->m_mesh->getVertexBuffer());
			deviceContext->setIndexBuffer(this->m_mesh->getIndexBuffer());

			deviceContext->drawIndexedTriangleList(this->m_mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
		}
	}

	void MeshRenderer::setMesh(Mesh* mesh)
	{
		m_mesh = mesh;
	}

	void MeshRenderer::setMeshFromFile(const wchar_t* filePath)
	{
		MeshManager* meshManager = GraphicsEngine::getInstance()->getMeshManager();
		this->m_mesh = meshManager->createMeshFromFile(filePath);
	}

	Mesh* MeshRenderer::getMesh()
	{
		return m_mesh;
	}
}
