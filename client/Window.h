#pragma once

#define NOMINMAX

#include <Windows.h>

namespace GDEngine
{
	class Window
	{
	protected:
		HWND m_windowHandle;
		bool m_isRunning = false;
		bool m_isInitialized = false;

	public:
		Window();
		~Window();

	public:
		virtual void onCreate();
		virtual void onUpdate();
		virtual void onDestroy();
		virtual void onFocus();
		virtual void onKillFocus();

		bool broadcast();
		bool isRunning();

		RECT getClientWindowRect();
	};
}