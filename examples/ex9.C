#include "ex9.hpp"

int main(int argc, char **argv)
{
	std::cout << "Example 9\n\n";
	std::cout << "This program finds the maximum value in the function\n";
	std::cout << "  y = - x1^2 - x2^2\n";
	std::cout << "with the constraints\n";
	std::cout << "     -5 <= x1 <= 5\n";
	std::cout << "     -5 <= x2 <= 5\n";
	std::cout << "\n\n";
	std::cout.flush();

	// See if we've been given a seed to use (for testing purposes).  When you
	// specify a random seed, the evolution will be exactly the same each time
	// you use that seed number.

	unsigned int seed = 0;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i++], "seed") == 0)
		{
			seed = atoi(argv[i]);
		}
	}

	example9(seed);

	return 0;
}
