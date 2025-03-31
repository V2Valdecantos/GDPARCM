#pragma once

#include <unordered_map>
#include <vector>

#include <Windows.h>

#include "UIScreen.h"

namespace GDEngine
{
class UINames {
public:
	const std::string MENU_SCREEN = "MENU_SCREEN";
	const std::string CREDITS_SCREEN = "CREDITS_SCREEN";
	const std::string PROFILER_SCREEN = "PROFILER_SCREEN";
	const std::string INSPECTOR_SCREEN = "INSPECTOR_SCREEN";
	const std::string HIERARCHY_SCREEN = "HIERARCHY_SCREEN";
	const std::string COLOR_PICKER_SCREEN = "COLOR_PICKER_SCREEN";
	const std::string VIEWPORT_SCREEN = "VIEWPORT_SCREEN";
	const std::string TOOLS_SCREEN = "TOOLS_SCREEN";
	const std::string ACTION_SCREEN = "ACTION_SCREEN";
	const std::string PLAYBACK_SCREEN = "PLAYBACK_SCREEN";
	const std::string LOGGER_SCREEN = "LOGGER_SCREEN";
};

	class UIManager
	{
	private:
		std::unordered_map<std::string, UIScreen*> mapUI;
		std::vector<UIScreen*> listUI;
		bool firstTime = true;
	public:
		static const int WINDOW_WIDTH = 1440;
		static const int WINDOW_HEIGHT = 900;

		static int RESIZE_WIDTH;
		static int RESIZE_HEIGHT;

		void draw();
		void drawDockspace();
		void setActive(std::string name);
		void setUIStyle();
		void addViewport(UIScreen* viewport);

		void AlignForWidth(float width, float alignment = 0.5f);

	private:
		static UIManager* P_SHARED_INSTANCE;

	private:
		UIManager(HWND hwnd);
		~UIManager();
		UIManager(const UIManager&);
		UIManager& operator = (const UIManager&);

	public:
		static UIManager* getInstance();
		static void initialize(HWND hwnd);
		static void destroy();
	};
}