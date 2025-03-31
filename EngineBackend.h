#pragma once

namespace GDEngine
{
	class EngineBackend
	{
	public:
		enum EditorMode
		{
			EDITOR	= 0,
			PLAY	= 1,
			PAUSED	= 2
		};
		
		static EngineBackend*	getInstance();
		static void	initialize();
		static void destroy();

		void setMode(EditorMode mode);
		void startFrameStep();
		void endFrameStep();
		bool insideFrameStep();
		EditorMode getMode();

	private:
		EngineBackend();
		EngineBackend(EngineBackend const&) {}
		EngineBackend& operator= (EngineBackend const&) {}
		~EngineBackend();

		static EngineBackend* P_SHARED_INSTANCE;

		EditorMode m_editorMode = EditorMode::EDITOR;
		bool m_frameStepping	= false;
	};
}