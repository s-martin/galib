/* ----------------------------------------------------------------------------
  ex17.C
  mbwall 5may95
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Example program illustrating the use of a 2DArrayGenome with trinary
alleles (-1, 0, 1).  The objective function for this program tries to
alternate 0 and 1 then put -1 in the corners.
---------------------------------------------------------------------------- */
#include "ex17.hpp"

int main(int argc, char *argv[])
{
	std::cout << "Example 17\n\n";
	std::cout << "This program illustrates the use of a 2DArrayGenome with\n";
	std::cout
		<< "three alleles.  It tries to fill a 2D array with alternating\n";
	std::cout
		<< "0s and 1s, and -1s at the corners.  You will have to run it\n";
	std::cout
		<< "for something like 10000 generations to get the perfect score.\n\n";
	std::cout.flush();

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

	example17(seed, argc, argv);

	return 0;
}
