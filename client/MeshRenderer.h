#pragma once

#include <string>

#include "GameObject.h"
#include "Renderer.h"

namespace GDEngine
{
	class MeshRenderer : public ARenderer
	{
	public:
		MeshRenderer(std::string name, AGameObject* owner);
		MeshRenderer(std::string guid, std::string name, AGameObject* owner);
		~MeshRenderer() override;

		virtual void perform(float deltaTime) override;

		void setMesh(Mesh* mesh);
		void setMeshFromFile(const wchar_t* filePath);

		Mesh* getMesh();

	private:
		Mesh* m_mesh;
	};
}
