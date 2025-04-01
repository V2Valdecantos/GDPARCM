#pragma once

#include <string>

namespace GDEngine
{
	class Resource
	{
	protected:
		std::wstring m_fullPath;

	public:
		Resource(const wchar_t* fullPath);
		virtual ~Resource();
	};
}