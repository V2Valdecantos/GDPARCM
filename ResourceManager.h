#pragma once

#include <unordered_map>
#include <string>

#include "Prerequisites.h"

namespace GDEngine {
	class ResourceManager
	{
	private:
		typedef std::unordered_map<std::wstring, Resource*> ResourceMap;

	protected:
		ResourceMap m_resourceMap;

	protected:
		ResourceManager();
		virtual ~ResourceManager();

	public:
		Resource* createResourceFromFile(const wchar_t* filePath);

	protected:
		virtual Resource* createResourceFromFileConcrete(const wchar_t* filePath) = 0;

	private:
		ResourceManager(ResourceManager const&);
		ResourceManager& operator=(ResourceManager const&);
	};
}