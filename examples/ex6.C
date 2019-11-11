/* ----------------------------------------------------------------------------
  ex6.C
  mbwall 14jan95
  Copyright 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Example program for the tree and list genomes.  This example contains
the code to run a tree genome.  The list genome is almost the same -
just change tree to list and modify the initialization methods.
  This program illustrates how to specialize member functions of the
template classes.  Here we specialize the default write() method so that we get
the contents of the nodes rather than the pointers to the node contents.  You
can specialize most functions of a template class (as long as they are not
inlined).
---------------------------------------------------------------------------- */

#include "ex6.hpp"


int main(int argc, char *argv[])
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
		if (strcmp(argv[i++], "seed") == 0)
		{
			seed = atoi(argv[i]);
		}
	}

	// Set the default values of the parameters.
	auto params = std::make_shared<GAParameterList>();
	params->set(gaNpopulationSize, 30);
	params->set(gaNpCrossover, 0.7);
	params->set(gaNpMutation, 0.01);
	params->set(gaNnGenerations, 100);
	params->set(gaNscoreFilename, "bog.dat");
	params->set(gaNscoreFrequency, 10); // record score every 10th generation
	params->set(gaNflushFrequency, 10); // dump scores every 10th recorded score
	params->parse(argc, argv); // Parse the command line for GAlib args.

	ex6(params, seed);

	return 0;
}
