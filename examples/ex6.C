#include "ex6.hpp"

#include <cstdlib>
#include <cstring>

int main(int argc, char **argv)
{
	std::cout << "Example 6\n\n";
	std::cout << "This example uses a SteadyState GA and Tree<int> genome.  It\n";
	std::cout << "tries to maximize the size of the tree genomes that it\n";
	std::cout << "contains.  The genomes contain ints in its nodes.\n\n";
	std::cout.flush();

	// See if we've been given a seed to use (for testing purposes).  When you
	// specify a random seed, the evolution will be exactly the same each time
	// you use that seed number.
	unsigned int seed = 0;
	for (int i = 1; i < argc; i++)
	{
		if (std::strcmp(argv[i], "seed") == 0 && i + 1 < argc)
		{
			seed = static_cast<unsigned int>(std::atoi(argv[i + 1]));
			i++;
		}
	}

	// Set the default values of the parameters.
	GAParameterList params;
	params.set(gaNpopulationSize, 30);
	params.set(gaNpCrossover, 0.7);
	params.set(gaNpMutation, 0.01);
	params.set(gaNnGenerations, 100);
	params.set(gaNscoreFilename, "bog.dat");
	params.set(gaNscoreFrequency, 10); // record score every 10th generation
	params.set(gaNflushFrequency, 10); // dump scores every 10th recorded score

	ex6(params, seed);
	return 0;
}
