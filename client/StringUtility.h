#pragma once
#include <string>
#include <vector>

#include <Windows.h>

namespace GDEngine {
	class StringUtility
	{
	public:
		static std::vector<std::string> split(const std::string& s, char delimiter);
		static GUID StringToGuid(const std::string& str);
		static std::string GuidToString(GUID guid);
	};
}

