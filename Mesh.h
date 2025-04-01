#pragma once

#include "Resource.h"

#include "Prerequisites.h"

namespace GDEngine
{
	class Mesh : public Resource
	{
	private:
		VertexBuffer* m_vertexBuffer;
		IndexBuffer* m_indexBuffer;
		std::string temp_filePath;
	private:
		friend class DeviceContext;

	public:
		Mesh(const wchar_t* fullPath);
		~Mesh() override;

		std::string getFilePath();

	public:
		VertexBuffer* getVertexBuffer();
		IndexBuffer* getIndexBuffer();
	};

}