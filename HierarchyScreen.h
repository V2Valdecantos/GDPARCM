#pragma once
#include "GameObject.h"
#include "UIScreen.h"

namespace GDEngine
{
	class HierarchyScreen : public UIScreen
	{
	private:
		HierarchyScreen();
		~HierarchyScreen();

	private:
		friend class UIManager;

	private:
		virtual void draw() override;
		void displayNode(AGameObject* gameObject);
		bool contains(std::string str, std::string substr);

	private:
		std::unordered_map<std::string, bool> m_expandedState;
	};
}