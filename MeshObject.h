#pragma once

#include "GameObject.h"

namespace GDEngine
{
	class MeshObject : public AGameObject
	{
	protected:
		Mesh* m_mesh;
		ConstantBuffer* m_constantBuffer;

	public:
		MeshObject(std::string name, const wchar_t* meshFilePath);
		MeshObject(std::string guid, std::string name, const wchar_t* meshFilePath);
		~MeshObject() override;

	public:
		void onCreate() override;
		void update(float deltaTime) override;
		void draw(int width, int height) override;
		void onDestroy() override;
	};
}
