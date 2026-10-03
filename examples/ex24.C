/* ----------------------------------------------------------------------------
  ex24.C
  mbwall 5jan96
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example shows how to restricted mating using a custom genetic algorithm
and custom selection scheme.  The restricted mating in the genetic algorithm
tries to pick individuals that are similar (based upon their comparator).  The
selector chooses only the upper half of the population (so it is very selective
and tends to drive the convergence faster than roulette wheel selection, for
example).
---------------------------------------------------------------------------- */
#include "ex24.hpp"

int main(int argc, char **argv)
{
  std::cout << "Example 24\n\n";
  std::cout << "This example illustrates how to derive your own genetic\n";
  std::cout << "algorithm.  This genetic algorithm does restricted mating and\n";
  std::cout << "uses a selector slightly more finicky than a uniform random\n";
  std::cout << "selector.  The objective function is a simple sinusoidal.\n\n";
  std::cout.flush();

  // See if we've been given a seed to use (for testing purposes).  When you
  // specify a random seed, the evolution will be exactly the same each time
  // you use that seed number.
  unsigned int seed = 0;
  for (int ii = 1; ii < argc; ii++)
  {
    if (strcmp(argv[ii++], "seed") == 0)
      seed = (unsigned int)atoi(argv[ii]);
  }

  example24(seed, argc, argv);

  return 0;
}
