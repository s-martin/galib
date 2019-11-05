/* ----------------------------------------------------------------------------
  ex1.C
  mbwall 28jul94
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Example program for the SimpleGA class and 2DBinaryStringGenome class.
   This program tries to fill the 2Dgenome with alternating 1s and 0s.
   This example uses the default crossover (single point), default mutator
   (uniform random bit flip), and default initializer (uniform random) for the
   2D genome.
   Notice that one-point crossover is not necessarily the best kind of crossover
   to use if you want to generate a 'good' genome with this kind of objective
   function.  But it does work.
---------------------------------------------------------------------------- */

#include "ex1.hpp"

#include <iostream>


int main(int argc, char **argv)
{
	std::cout << "Example 1\n\n";
	std::cout << "This program tries to fill a 2DBinaryStringGenome with\n";
	std::cout << "alternating 1s and 0s using a SimpleGA\n\n";
	std::cout.flush();

	auto ga = ex1();

	// Now we print out the best genome that the GA found.
	std::cout << "The GA found:\n" << ga.statistics().bestIndividual() << "\n";

	// That's it!
	return 0;
}
