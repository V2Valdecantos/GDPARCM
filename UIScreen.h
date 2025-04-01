#pragma once

#include <string>

namespace GDEngine
{
	class UIManager;
	class UIScreen
	{
	protected:
		std::string name;
		bool isActive = true;

	protected:
		friend class UIManager;

	protected:
		UIScreen(std::string name);
		~UIScreen();

	protected:
		std::string getName();
		virtual void draw() = 0;
		void setActive(bool isActive);
	};
}