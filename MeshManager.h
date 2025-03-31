#pragma once

#include "ResourceManager.h"

namespace GDEngine
{
	class MeshManager : public ResourceManager
	{
	public:
		Mesh* createMeshFromFile(const wchar_t* filePath);

	protected:
		virtual Resource* createResourceFromFileConcrete(const wchar_t* filePath);
		friend class GraphicsEngine;

	private:
		MeshManager();
		~MeshManager() override;
	};
}