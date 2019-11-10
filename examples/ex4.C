/* ----------------------------------------------------------------------------
  ex4.C
  mbwall 28jul94
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Example program for the SteadyStateGA class and 3DBinaryStringGenome
   class.  This program tries to fill the 3Dgenome with alternating 1's and
   0's.  Notice that the steady state ga needs many more 'generations' than the
   simple ga, but it will usually converge to the optimum with fewer evaluations
   of the objective function.  So if you have a compute-intensive objective
   function, steady state ga may be the way to go.
   You can control the amount of overlap in a steady-state GA by using the
   pReplacement member function.
   This example also illustrates some of the statistics members of the genetic
   algorithm object.
---------------------------------------------------------------------------- */

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
	for (int ii = 1; ii < argc; ii++)
	{
		if (strcmp(argv[ii++], "seed") == 0)
		{
			GARandomSeed((unsigned int)atoi(argv[ii]));
		}
	}

	ex4();

	return 0;
}
