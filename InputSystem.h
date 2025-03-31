#pragma once

#include <unordered_set>

#include "InputListener.h"
#include "Vector2D.h"

namespace GDEngine
{
	class InputSystem
	{
	private:
		std::unordered_set<InputListener*> m_setListeners;
		unsigned char m_keysState[256] = {};
		unsigned char m_oldKeysState[256] = {};

		Vector2D m_oldMousePosition;
		Vector2D m_deltaMousePosition;

		bool m_firstTime = true;
		bool m_shouldUpdate = true;

	public:
		void update();
		void stopUpdate();
		void startUpdate();
		void addListener(InputListener* listener);
		void removeListener(InputListener* listener);
		void setCursorPosition(const Vector2D& position);
		void showCursor(bool show);

		bool getKeyDown(int key);
		bool getKey(int key);
		bool getKeyUp(int key);
		Vector2D getDeltaMousePosition();
		Vector2D getMousePosition();
		Vector2D getOldMousePosition();

	private:
		static InputSystem* P_SHARED_INSTANCE;

	private:
		InputSystem();
		~InputSystem();
		InputSystem(InputSystem const&);
		InputSystem& operator = (InputSystem const&);

	public:
		static InputSystem* getInstance();
		static void initialize();
		static void destroy();
	};
}