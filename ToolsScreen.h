#pragma once

#include "UIScreen.h"

namespace GDEngine
{
	class ToolsScreen : public UIScreen
	{
	private:
		ToolsScreen();
		~ToolsScreen();

	private:
		friend class UIManager;

	private:
		virtual void draw() override;
	};
}