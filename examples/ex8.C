#include "ex8.hpp"

int main(int argc, char *argv[])
{
	std::cout << "Example 8\n\n";
	std::cout << "This program runs a steady-state GA whose objective function\n";
	std::cout << "tries to maximize the size of the list and tries to make lists\n";
	std::cout << "that contain the number 101 in the nodes.  The lists contain\n";
	std::cout << "ints in the nodes.\n\n";
	std::cout.flush();

	// See if we've been given a seed to use (for testing purposes).  When you
	// specify a random seed, the evolution will be exactly the same each time
	// you use that seed number.
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i++], "seed") == 0)
			GARandomSeed((unsigned int)atoi(argv[i]));
	}

	example8(0);

	return 0;
}
