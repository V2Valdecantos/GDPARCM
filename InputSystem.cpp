#include "InputSystem.h"
#include <Windows.h>

#include "Logger.h"

using namespace GDEngine;

InputSystem* InputSystem::P_SHARED_INSTANCE = nullptr;

void InputSystem::update()
{
	if (m_shouldUpdate)
	{
		POINT currentMousePosition = {};
		::GetCursorPos(&currentMousePosition);

		if (m_firstTime)
		{
			m_firstTime = false;
			m_oldMousePosition = Vector2D(currentMousePosition.x, currentMousePosition.y);
			m_deltaMousePosition = Vector2D::zero();
		}

		if (currentMousePosition.x != m_oldMousePosition.x || currentMousePosition.y != m_oldMousePosition.y)
		{
			std::unordered_set<InputListener*>::iterator it = m_setListeners.begin();
			m_deltaMousePosition = Vector2D(currentMousePosition.x - m_oldMousePosition.x, currentMousePosition.y - m_oldMousePosition.y);
			while (it != m_setListeners.end()) {
				//(*it)->onMouseMove(Vector2D(currentMousePosition.x - m_oldMousePosition.x, currentMousePosition.y - m_oldMousePosition.y));
				(*it)->onMouseMove(Vector2D(currentMousePosition.x, currentMousePosition.y));
				++it;
			}
		}
		else
			m_deltaMousePosition = Vector2D::zero();

		m_oldMousePosition = Vector2D(currentMousePosition.x, currentMousePosition.y);

		if (::GetKeyboardState(m_keysState))
		{
			for (unsigned int i = 0; i < 256; i++)
			{
				// Key is down
				if (m_keysState[i] & 0x80)
				{
					std::unordered_set<InputListener*>::iterator it = m_setListeners.begin();
					while (it != m_setListeners.end()) {
						if (i == VK_LBUTTON)
						{
							if (m_keysState[i] != m_oldKeysState[i])
								(*it)->onLeftMouseDown(Vector2D(currentMousePosition.x, currentMousePosition.y));
						}
						else if (i == VK_RBUTTON)
						{
							if (m_keysState[i] != m_oldKeysState[i])
								(*it)->onRightMouseDown(Vector2D(currentMousePosition.x, currentMousePosition.y));
						}
						else
							(*it)->onKeyDown(i);

						++it;
					}
				}
				else // Key is up
				{
					if (m_keysState[i] != m_oldKeysState[i])
					{
						std::unordered_set<InputListener*>::iterator it = m_setListeners.begin();

						while (it != m_setListeners.end())
						{
							if (i == VK_LBUTTON)
								(*it)->onLeftMouseUp(Vector2D(currentMousePosition.x, currentMousePosition.y));
							else if (i == VK_RBUTTON)
								(*it)->onRightMouseUp(Vector2D(currentMousePosition.x, currentMousePosition.y));
							else
								(*it)->onKeyUp(i);
							++it;
						}
					}
				}
			}
			// Store current key state to old key state
			::memcpy(m_oldKeysState, m_keysState, sizeof(unsigned char) * 256);
		}
	}
}

void InputSystem::stopUpdate()
{
	m_shouldUpdate = false;
}

void InputSystem::startUpdate()
{
	m_shouldUpdate = true;
}

void InputSystem::addListener(InputListener* listener)
{
	m_setListeners.insert(listener);
}

void InputSystem::removeListener(InputListener* listener)
{
	m_setListeners.erase(listener);
}

void InputSystem::setCursorPosition(const Vector2D& position)
{
	::SetCursorPos(position.x, position.y);
}

void InputSystem::showCursor(bool show)
{
	::ShowCursor(show);
}

bool InputSystem::getKeyDown(int key)
{
	if (::GetKeyboardState(m_keysState))
		if (m_keysState[key] & 0x80 && m_keysState[key] != m_oldKeysState[key])
		{
			::memcpy(m_oldKeysState, m_keysState, sizeof(unsigned char) * 256);
			return true;
		}
	return false;
}

bool InputSystem::getKey(int key)
{
	if (::GetKeyboardState(m_keysState))
		if (m_keysState[key] & 0x80)
		{
			::memcpy(m_oldKeysState, m_keysState, sizeof(unsigned char) * 256);
			return true;
		}
	return false;
}

bool InputSystem::getKeyUp(int key)
{
	if (::GetKeyboardState(m_keysState))
	{
		if (m_keysState[key] & 0x80) {}

		else
		{
			if (m_keysState[key] != m_oldKeysState[key])
			{
				::memcpy(m_oldKeysState, m_keysState, sizeof(unsigned char) * 256);
				return true;
			}
		}
	}
	return false;
}

Vector2D InputSystem::getMousePosition()
{
	POINT currentMousePosition = {};
	::GetCursorPos(&currentMousePosition);
	return Vector2D(currentMousePosition.x, currentMousePosition.y);
}

Vector2D InputSystem::getOldMousePosition()
{
	return m_oldMousePosition;
}

Vector2D InputSystem::getDeltaMousePosition()
{
	return m_deltaMousePosition;
}

InputSystem::InputSystem()
{
	Logger::log(this, "Initialized");
}
InputSystem::~InputSystem()
{
	P_SHARED_INSTANCE = nullptr;
	Logger::log(P_SHARED_INSTANCE, "Destroyed");
}
InputSystem::InputSystem(const InputSystem&) {}

InputSystem* InputSystem::getInstance()
{
	return P_SHARED_INSTANCE;
}

void InputSystem::initialize()
{
	if (P_SHARED_INSTANCE)
	{
		Logger::throw_exception("Input System already created");
	}
	P_SHARED_INSTANCE = new InputSystem();
}

void InputSystem::destroy()
{
	delete P_SHARED_INSTANCE;
}