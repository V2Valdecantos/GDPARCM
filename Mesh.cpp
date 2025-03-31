#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>

#include <locale>
#include <codecvt>
#include <filesystem>

#include "GraphicsEngine.h"
#include "Logger.h"
#include "ShaderLibrary.h"
#include "VertexMesh.h"
#include "TexturedVertexBuffer.h"

namespace GDEngine {
	Mesh::Mesh(const wchar_t* fullPath) : Resource(fullPath)
	{
		tinyobj::attrib_t attribs;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn;
		std::string err;

		std::string inputFile = std::filesystem::path(fullPath).string();
		temp_filePath = inputFile;
		Logger::log(L"Creating mesh from : " + std::wstring(fullPath));
		bool result = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputFile.c_str());

		if (!err.empty())
			Logger::throw_exception("Mesh not created successfully.");

		if (!result)
			Logger::throw_exception("Mesh not created successfully.");

		if (shapes.size() > 1)
			Logger::throw_exception("Mesh not created successfully.");

		std::vector<VertexMesh> listVertices;
		std::vector<unsigned int> listIndices;

		for (size_t s = 0; s < shapes.size(); s++)
		{
			size_t indexOffset = 0;
			listVertices.reserve(shapes[s].mesh.indices.size());
			listIndices.reserve(shapes[s].mesh.indices.size());

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				unsigned char numFaceVertices = shapes[s].mesh.num_face_vertices[f];

				for (unsigned char v = 0; v < numFaceVertices; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[indexOffset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if (!attribs.texcoords.empty())
					{
						tx = attribs.texcoords[index.texcoord_index * 2 + 0];
						ty = attribs.texcoords[index.texcoord_index * 2 + 1];
					}

					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty));
					listVertices.push_back(vertex);

					listIndices.push_back(indexOffset + v);
				}
				indexOffset += numFaceVertices;
			}
		}

		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

		ShaderNames shaderNames;

		void* shaderByteCode = NULL;
		size_t sizeShader = 0;

		ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.VERTEX_MESH_LAYOUT_SHADER_NAME, &shaderByteCode, &sizeShader);

		Logger::log("Trying to create Mesh Textured Vertex Buffer");
		m_vertexBuffer = renderSystem->createTexturedVertexBuffer(&listVertices[0], sizeof(VertexMesh), (UINT)listVertices.size(), shaderByteCode, sizeShader);
		Logger::log("Trying to create Mesh Index Buffer");
		m_indexBuffer = renderSystem->createIndexBuffer(&listIndices[0], (UINT)listIndices.size());

		Logger::log("Mesh created successfully");
	}

	Mesh::~Mesh()
	{
		delete this->m_indexBuffer;
		delete this->m_vertexBuffer;
	}

	std::string Mesh::getFilePath()
	{
		return temp_filePath;
	}

	VertexBuffer* Mesh::getVertexBuffer()
	{
		return this->m_vertexBuffer;
	}

	IndexBuffer* Mesh::getIndexBuffer()
	{
		return this->m_indexBuffer;
	}
}