#pragma once

#include <imgui.h>
#include "imfilebrowser.h"

#include "UIScreen.h"

namespace GDEngine
{
	class MenuScreen : public UIScreen
	{
	private:
		bool m_openSceneSelected = false;

		ImGui::FileBrowser* m_saveSceneDialog;
		ImGui::FileBrowser* m_openSceneDialog;
	private:
		MenuScreen();
		~MenuScreen();

	private:
		friend class UIManager;

	private:
		virtual void draw() override;
	};
}

