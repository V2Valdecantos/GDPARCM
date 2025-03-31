#include "EngineTime.h"

#include "Logger.h"

namespace GDEngine
{
	EngineTime* EngineTime::P_SHARED_INSTANCE = NULL;

	EngineTime::EngineTime()
	{
		Logger::log(P_SHARED_INSTANCE, "Initialized");
	}
	EngineTime::~EngineTime() {}
	EngineTime::EngineTime(EngineTime const&) {}

	void EngineTime::initialize()
	{
		if (P_SHARED_INSTANCE)
		{
			Logger::throw_exception("Engine Time already created");
		}
		P_SHARED_INSTANCE = new EngineTime();
	}

	double EngineTime::getDeltaTime()
	{
		return P_SHARED_INSTANCE->m_deltaTime;
	}

	void EngineTime::LogFrameStart()
	{
		P_SHARED_INSTANCE->m_start = std::chrono::system_clock::now();
	}

	void EngineTime::LogFrameEnd()
	{
		P_SHARED_INSTANCE->m_end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = P_SHARED_INSTANCE->m_end - P_SHARED_INSTANCE->m_start;

		P_SHARED_INSTANCE->m_deltaTime = elapsed_seconds.count();
	}
}