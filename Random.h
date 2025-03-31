#pragma once

#include <random>

namespace GDEngine
{
	class Random
	{
	private:
		std::mt19937 m_numberGenerator;

	private:
		static Random* P_SHARED_INSTANCE;

	public:
		static int range(int minInclusive, int maxExclusive);
		static float range(float minInclusive, float maxInclusive);

	private:
		Random();
		~Random();
		Random(Random const&);
		Random& operator = (Random const&);

	public:
		static void initialize();
		static void destroy();
	};
}