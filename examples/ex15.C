/* ----------------------------------------------------------------------------
  ex15.C
  mbwall 28jul94
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example nearly identical to example 2, but it uses convergence as the
stopping criterion for the GA rather than number-of-generations.
---------------------------------------------------------------------------- */
#include <cstdio>
#include <cmath>
#include <ga.h>
#include "ex15.hpp"

#include <iostream>

 

int main(int argc, char **argv)
{
  std::cout << "Example 15\n\n";
  std::cout << "This program generates a sequence of random numbers then uses\n";
  std::cout << "a simple GA and binary-to-decimal genome to match the\n";
  std::cout << "sequence.  It uses the convergence of the best-of-generation\n";
  std::cout << "as the criterion for when to stop.\n\n";

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  unsigned int seed = 0;
  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii++],"seed") == 0) {
      seed = atoi(argv[ii]);
    }
  }

  GAStatistics stats = example15(seed, argc, argv);

// Dump the results of the GA to the screen.

  GABin2DecGenome genome = stats.bestIndividual();
  std::cout << "the ga generated:\n";
  for(unsigned int jj=0; jj<genome.nPhenotypes(); jj++){
    std::cout.width(10); std::cout << genome.phenotype(jj) << " ";
  }
  std::cout << "\n\n"; std::cout.flush();

  return 0;
}
