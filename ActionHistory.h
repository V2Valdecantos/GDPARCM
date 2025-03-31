#pragma once

#include <stack>

namespace GDEngine
{
	class EditorAction;
	class AGameObject;
	class ActionHistory
	{
	public:
		typedef std::stack<EditorAction*> ActionStack;

		static ActionHistory* getInstance();
		static void initialize();
		static void destroy();

		void recordAction(AGameObject* gameObject);
		bool hasRemainingUndoActions();
		bool hasRemainingRedoActions();
		EditorAction* undoAction();
		EditorAction* redoAction();
		void clear();

	private:
		ActionHistory();
		ActionHistory(ActionHistory const&) {}
		ActionHistory& operator=(ActionHistory const&) {}
		~ActionHistory();

		static ActionHistory* P_SHARED_INSTANCE;

		ActionStack m_actionsPerformed;
		ActionStack m_actionsCancelled;
	};
}
