#include "ActionHistory.h"

#include "EditorAction.h"
#include "EngineBackend.h"
#include "Logger.h"

namespace GDEngine
{
	ActionHistory* ActionHistory::P_SHARED_INSTANCE = nullptr;
	ActionHistory* ActionHistory::getInstance()
	{
		return P_SHARED_INSTANCE;
	}
	void ActionHistory::initialize()
	{
		if (P_SHARED_INSTANCE)
		{
			Logger::throw_exception("Input System already created");
		}
		P_SHARED_INSTANCE = new ActionHistory();
	}
	void ActionHistory::destroy()
	{
		delete P_SHARED_INSTANCE;
	}

	void ActionHistory::recordAction(AGameObject* gameObject)
	{
		if (EngineBackend::getInstance()->getMode() == EngineBackend::EditorMode::EDITOR)
		{
			EditorAction* editorAction = new EditorAction(gameObject);

			Logger::log(std::to_string(gameObject->getLocalPosition().x) + ", " + std::to_string(gameObject->getLocalPosition().y) + std::to_string(gameObject->getLocalPosition().z));


			this->m_actionsPerformed.push(editorAction);

			Logger::log("Stored Action " + gameObject->getName());
		}
	}

	bool ActionHistory::hasRemainingUndoActions()
	{
		return !m_actionsPerformed.empty();
	}

	bool ActionHistory::hasRemainingRedoActions()
	{
		return !m_actionsCancelled.empty();
	}

	EditorAction* ActionHistory::undoAction()
	{
		if (EngineBackend::getInstance()->getMode() != EngineBackend::EditorMode::EDITOR)
		{
			Logger::log("Cannot perform undo action during play.");
			return nullptr;
		}

		if (this->hasRemainingUndoActions())
		{
			EditorAction* action = m_actionsPerformed.top();
			m_actionsPerformed.pop();
			m_actionsCancelled.push(action);
			//Logger::log("Redo actions: " + std::to_string(m_actionsCancelled.size()));
			return action;
		}

		Logger::log("No more actions remaining.");
		return nullptr;
	}

	EditorAction* ActionHistory::redoAction()
	{
		if (EngineBackend::getInstance()->getMode() != EngineBackend::EditorMode::EDITOR)
		{
			Logger::log("Cannot perform redo action during play.");
			return nullptr;
		}

		if (this->hasRemainingRedoActions())
		{
			EditorAction* action = m_actionsCancelled.top();

			m_actionsCancelled.pop();
			m_actionsPerformed.push(action);
			return action;
		}

		Logger::log("No more actions remaining.");
		return nullptr;
	}

	void ActionHistory::clear()
	{
		for (int i = 0; i < m_actionsCancelled.size(); i++)
		{
			m_actionsCancelled.pop();
		}
	}

	ActionHistory::ActionHistory()
	{
		Logger::log(this, "Initialized");
	}

	ActionHistory::~ActionHistory()
	{
		Logger::log(this, "Destroyed");
	}
}
