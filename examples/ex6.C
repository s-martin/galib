#include "ex6.hpp"

GAStatistics example6(GAParameterList params, unsigned int seed)
{
	std::cout << "Example 6\n\n";
	std::cout << "This example uses a SteadyState GA and Tree<int> genome.  It\n";
	std::cout << "tries to maximize the size of the tree genomes that it\n";
	std::cout << "contains.  The genomes contain ints in its nodes.\n\n";
	std::cout.flush();

	// See if we've been given a seed to use (for testing purposes).  When you
	// specify a random seed, the evolution will be exactly the same each time
	// you use that seed number.

	// Set the default values of the parameters.
	params.set(gaNpopulationSize, 30);
	params.set(gaNpCrossover, 0.7);
	params.set(gaNpMutation, 0.01);
	params.set(gaNnGenerations, 100);
	params.set(gaNscoreFilename, "bog.dat");
	params.set(gaNscoreFrequency, 10); // record score every 10th generation
	params.set(gaNflushFrequency, 10); // dump scores every 10th recorded score

	return ex6(params, seed);
}
