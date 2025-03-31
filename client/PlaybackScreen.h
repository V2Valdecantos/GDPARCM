#pragma once

#include "UIScreen.h"

namespace GDEngine {
	class PlaybackScreen : public UIScreen
	{
	private:
		PlaybackScreen();
		~PlaybackScreen();

	private:
		friend class UIManager;

	private:
		virtual void draw() override;
	};
}