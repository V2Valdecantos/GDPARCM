#pragma once

#include "UIScreen.h"

namespace GDEngine
{
	class ColorPickerScreen : public UIScreen
	{
	private:
		float color[4] = { 0,0,0,0 };
	private:
		ColorPickerScreen();
		~ColorPickerScreen();

	private:
		friend class UIManager;

	private:
		virtual void draw() override;
	};
}