/* ----------------------------------------------------------------------------
  ex2.C
  mbwall 28jul94
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Example program for the SimpleGA class and Bin2DecGenome class.  This
program generates randomly a series of numbers then tries to match those
values in a binary-to-decimal genome.  We use a simple GA (with linear
scaled fitness selection and non-steady-state population generation) and
binary-to-decimal, 1D genomes.  We also use the userData argument to the
objective function.
---------------------------------------------------------------------------- */

#include "ex2.hpp"


int main(int argc, char **argv)
{
	std::cout << "Example 2\n\n";
	std::cout << "This program generates a sequence of random numbers then uses\n";
	std::cout << "a simple GA and binary-to-decimal genome to match the\n";
	std::cout << "sequence.\n\n";

	// See if we've been given a seed to use (for testing purposes).  When you
	// specify a random seed, the evolution will be exactly the same each time
	// you use that seed number.
	unsigned int seed = 0;
	for (int ii = 1; ii < argc; ii++)
	{
		if (strcmp(argv[ii++], "seed") == 0)
		{
			seed = atoi(argv[ii]);
		}
	}

	GARandomSeed(seed);

	ex2(seed, false); // use dynamic values

	return 0;
}
