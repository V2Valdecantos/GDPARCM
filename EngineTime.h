#pragma once

#include <chrono>

namespace GDEngine
{
	class EngineTime
	{
	private:
		std::chrono::system_clock::time_point m_start;
		std::chrono::system_clock::time_point m_end;

		double m_deltaTime = 0.0f;

	private:
		static EngineTime* P_SHARED_INSTANCE;

	private:
		EngineTime();
		~EngineTime();
		EngineTime(EngineTime const&);
		EngineTime& operator = (EngineTime const&);

	private:
		friend class Window;

	public:
		static void initialize();
		static double getDeltaTime();

	private:
		static void LogFrameStart();
		static void LogFrameEnd();
	};
}