#include "EngineBackend.h"

#include "GameObjectManager.h"
#include "Logger.h"

namespace GDEngine
{
	EngineBackend* EngineBackend::P_SHARED_INSTANCE = nullptr;
	EngineBackend* EngineBackend::getInstance()
	{
		return P_SHARED_INSTANCE;
	}

	void EngineBackend::initialize()
	{
		if (P_SHARED_INSTANCE)
		{
			Logger::throw_exception("Input System already created");
		}
		P_SHARED_INSTANCE = new EngineBackend();
	}

	void EngineBackend::destroy()
	{
		delete P_SHARED_INSTANCE;
	}

	void EngineBackend::setMode(EditorMode mode)
	{
		m_editorMode = mode;
		if (m_editorMode == EditorMode::PLAY)
		{
			GameObjectManager::getInstance()->saveEditStates();
		}
		else if (m_editorMode == EditorMode::EDITOR)
		{
			GameObjectManager::getInstance()->restoreEditStates();
		}
	}

	void EngineBackend::startFrameStep()
	{
		m_frameStepping = true;
	}

	void EngineBackend::endFrameStep()
	{
		m_frameStepping = false;
	}

	bool EngineBackend::insideFrameStep()
	{
		return m_frameStepping;
	}

	EngineBackend::EditorMode EngineBackend::getMode()
	{
		return this->m_editorMode;
	}

	EngineBackend::EngineBackend()
	{
		Logger::log(this, "Initialized");
	}

	EngineBackend::~EngineBackend()
	{
		Logger::log(this, "Destroyed");
	}
}