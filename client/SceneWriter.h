#pragma once
#include <string>

namespace GDEngine {
	class SceneWriter
	{
	public:
		SceneWriter(std::string directory);
		~SceneWriter();

		void writeToFile();
		void writeToJson();

	private:
		std::string m_directory;
	};
}
