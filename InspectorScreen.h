#pragma once
#include "GameObject.h"
#include "imgui.h"
#include "imfilebrowser.h"
#include "UIScreen.h"

namespace GDEngine
{
	class InspectorScreen : public UIScreen
	{
	private:
		friend class UIManager;

		ImGui::FileBrowser* m_meshDialog;

	private:
		InspectorScreen();
		~InspectorScreen();

	private:
		virtual void draw() override;
		void drawInspector();
		void drawSharedInspector(std::vector<AGameObject*> objects);
		void drawTransformTable(AGameObject* gameObject);
		void drawSharedTransformTable(std::vector<AGameObject*> objects);
		void drawComponentList(AGameObject* gameObject);

	private:
		AGameObject* m_selectedObject = nullptr;
		float m_position[3];
		float m_scale[3];
		float m_rotation[3];
		bool m_hasChanged = false;
		bool m_isLeftDown = false;
		bool m_addComponent = false;

	};
}