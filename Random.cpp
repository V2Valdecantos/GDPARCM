#include "Random.h"

#include "Logger.h"

namespace GDEngine
{
	int Random::range(int minInclusive, int maxExclusive)
	{
		std::uniform_int_distribution<int> randomizer(minInclusive, maxExclusive);
		return randomizer(P_SHARED_INSTANCE->m_numberGenerator);
	}

	float Random::range(float minInclusive, float maxInclusive)
	{
		std::uniform_real_distribution<float> randomizer(minInclusive, std::nextafter(maxInclusive, FLT_MAX));
		return randomizer(P_SHARED_INSTANCE->m_numberGenerator);
	}

	Random* Random::P_SHARED_INSTANCE = NULL;

	Random::Random() : m_numberGenerator(std::random_device{}())
	{
		m_numberGenerator.seed(time(nullptr));
		Logger::log(P_SHARED_INSTANCE, "Initialized");
	}
	Random::~Random()
	{
		Logger::log(P_SHARED_INSTANCE, "Destroyed");
	}
	Random::Random(Random const&) {}


	void Random::initialize()
	{
		if (P_SHARED_INSTANCE)
		{
			Logger::throw_exception("Random already created");
		}
		P_SHARED_INSTANCE = new Random();
	}

	void Random::destroy()
	{
		delete P_SHARED_INSTANCE;
	}
}