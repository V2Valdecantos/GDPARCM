#include "StringUtility.h"

#include <string>
#include <sstream>
#include <vector>

namespace GDEngine {
	std::vector<std::string> StringUtility::split(const std::string& s, char delimiter)
	{
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> tokens;
        while (std::getline(ss, item, delimiter))
        {
            tokens.push_back(item);
        }

        return tokens;
	}

	GUID StringUtility::StringToGuid(const std::string& str)
	{
		GUID guid;
		sscanf(str.c_str(),
			"{%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx}",
			&guid.Data1, &guid.Data2, &guid.Data3,
			&guid.Data4[0], &guid.Data4[1], &guid.Data4[2], &guid.Data4[3],
			&guid.Data4[4], &guid.Data4[5], &guid.Data4[6], &guid.Data4[7]);

		return guid;
	}

	std::string StringUtility::GuidToString(GUID guid)
	{
		char guid_cstr[39];
		snprintf(guid_cstr, sizeof(guid_cstr),
			"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

		return std::string(guid_cstr);
	}
}
