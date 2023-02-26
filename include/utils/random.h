#pragma once

#include <random>
#include <vector>

namespace utils
{
	unsigned int rand()
	{
		return std::random_device()();
	}

	unsigned int rand(unsigned int min, unsigned int max)
	{
		return min + (rand() % (max - min + 1));
	}
}
