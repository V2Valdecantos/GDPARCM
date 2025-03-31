#include "Window.h"

#include "imgui.h"

#include "EngineTime.h"
#include "Logger.h"
#include "UIManager.h"

using namespace GDEngine;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(windowHandle, msg, wParam, lParam))
	{
		return true;
	}

	switch (msg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		UIManager::RESIZE_WIDTH = (UINT)LOWORD(lParam); // Queue resize
		UIManager::RESIZE_HEIGHT = (UINT)HIWORD(lParam);
	case WM_CREATE:
	{
		// Event fired when the window is created
		// collected here..

		break;
	}
	case WM_SETFOCUS:
	{
		// Event fired when the window gets focus
		Window* window = (Window*)GetWindowLongPtr(windowHandle, GWLP_USERDATA);
		if (window)
			window->onFocus();
		break;
	}
	case WM_KILLFOCUS:
	{
		// Event fired when the window loses focus
		Window* window = (Window*)GetWindowLongPtr(windowHandle, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}
	case WM_DESTROY:
	{
		// Event fired when the window is destroyed
		Window* window = (Window*)GetWindowLongPtr(windowHandle, GWLP_USERDATA);
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}
	case WM_DPICHANGED:
	{
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
		{
			const RECT* suggestedRect = (RECT*)lParam;
			::SetWindowPos(windowHandle, nullptr, suggestedRect->left, suggestedRect->top, suggestedRect->right - suggestedRect->left, suggestedRect->bottom - suggestedRect->top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
		break;
	}
	default:
		return ::DefWindowProc(windowHandle, msg, wParam, lParam);
	}
}

namespace GDEngine {
	Window::Window()
	{
		WNDCLASSEX wc;
		wc.cbClsExtra = NULL;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wc.hInstance = NULL;
		wc.lpszClassName = L"MyWindowClass";
		wc.lpszMenuName = L"";
		wc.style = NULL;
		wc.lpfnWndProc = &WndProc;

		if (!::RegisterClassEx(&wc))
		{
			Logger::throw_exception("Window not created successfully");
		}

		m_windowHandle = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"[GROUP 2] DirectXApplication", WS_OVERLAPPEDWINDOW, 0, 0, UIManager::WINDOW_WIDTH, UIManager::WINDOW_HEIGHT,
			NULL, NULL, NULL, NULL);

		if (!m_windowHandle)
		{
			Logger::throw_exception("Window not created successfully");
		}

		::ShowWindow(m_windowHandle, SW_SHOW);
		::UpdateWindow(m_windowHandle);

		m_isRunning = true;

		EngineTime::initialize();
	}

	Window::~Window()
	{
	}

	bool Window::broadcast()
	{
		if (!this->m_isInitialized)
		{
			SetWindowLongPtr(m_windowHandle, GWLP_USERDATA, (LONG_PTR)this);
			this->onCreate();
			this->m_isInitialized = true;
		}

		EngineTime::LogFrameStart();
		this->onUpdate();

		MSG msg;
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Sleep(1);
		EngineTime::LogFrameEnd();

		return true;
	}

	bool Window::isRunning()
	{
		return m_isRunning;
	}

	RECT Window::getClientWindowRect()
	{
		RECT rect;
		::GetClientRect(this->m_windowHandle, &rect);
		::ClientToScreen(this->m_windowHandle, (POINT*)&rect);

		return rect;
	}

	void Window::onCreate()
	{
	}

	void Window::onUpdate()
	{

	}

	void Window::onDestroy()
	{
		m_isRunning = false;
	}

	void Window::onFocus()
	{
	}

	void Window::onKillFocus()
	{
	}
}