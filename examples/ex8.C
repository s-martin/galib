/* ----------------------------------------------------------------------------
  ex8.C
  mbwall 14jan95
  Copyright 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Example program for the list genome.  This example contains
the code to run a genetic algorithm with a list genome.
  This program illustrates how to specialize member functions of the
template classes.  Here we specialize the default write() method so that we get
the contents of the nodes rather than the pointers to the node contents.  You
can specialize most functions of a template class (as long as they are not
inlined).
  The objective function for this example returns both positive and negative
values, depending on the genome it is evaluting.  So the example also shows how
to use the sigma truncation scaling method to handle the mixed scores.
---------------------------------------------------------------------------- */

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

	ex8();

	return 0;
}
