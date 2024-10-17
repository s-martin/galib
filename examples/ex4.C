#include "ex4.hpp"

#include <iostream>


int main(int argc, char **argv)
{
	std::cout << "Example 4\n\n";
	std::cout << "This program tries to fill a 3DBinaryStringGenome with\n";
	std::cout << "alternating 1s and 0s using a SteadyStateGA\n\n";
	std::cout.flush();

	// See if we've been given a seed to use (for testing purposes).  When you
	// specify a random seed, the evolution will be exactly the same each time
	// you use that seed number.
	unsigned int seed = 0;
	for (int ii = 1; ii < argc; ii++)
	{
		if (strcmp(argv[ii++], "seed") == 0)
		{
			seed = (unsigned int)atoi(argv[ii]);
		}
	}

	example4(seed);

	return 0;
}
