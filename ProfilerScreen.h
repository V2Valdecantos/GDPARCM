#pragma once

#include "UIScreen.h"

namespace GDEngine
{
	class ProfilerScreen : public UIScreen
	{
	private:
		ProfilerScreen();
		~ProfilerScreen();

	private:
		friend class UIManager;

	private:
		virtual void draw() override;
	};
}