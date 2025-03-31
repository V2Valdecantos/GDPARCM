#include "MeshManager.h"

#if __cplusplus <= 201402L 
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#endif

#if __cplusplus >= 201703L
#include <filesystem>
#endif

#include "Logger.h"
#include "Mesh.h"

using namespace GDEngine;

MeshManager::MeshManager() : ResourceManager()
{
	Logger::log(this, "Initialized");
}

MeshManager::~MeshManager()
{
	Logger::log(this, "Destroyed");
}

Mesh* MeshManager::createMeshFromFile(const wchar_t* filePath)
{
#if (_MSC_VER >= 1900 && _MSC_VER <= 1916)  || ( _MSC_VER >= 1920 && __cplusplus <= 201402L) 
	std::wstring fullPath = std::experimental::filesystem::absolute(filePath);
#endif

#if _MSC_VER >= 1920 && __cplusplus > 201402L 
	std::wstring m_fullPath = std::filesystem::absolute(filePath);
#endif

	if (this->m_resourceMap[fullPath] == NULL)
	{
		Logger::log(L"Trying to create Mesh from path : " + std::wstring(filePath));
		this->m_resourceMap[fullPath] = this->createResourceFromFileConcrete(filePath);
	}

	return (Mesh*)this->m_resourceMap[fullPath];
}

Resource* MeshManager::createResourceFromFileConcrete(const wchar_t* filePath)
{
	Mesh* mesh = nullptr;
	try
	{
		mesh = new Mesh(filePath);
	}
	catch (...) {}

	return mesh;
}