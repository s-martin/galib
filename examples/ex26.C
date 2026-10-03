/* ----------------------------------------------------------------------------
  ex26.C
  mbwall 24mar96
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

  The code for this example is adapted from an original implementation 
  by Thomas Grueninger (from Uni Stuttgart, visiting scholar at MIT)

 DESCRIPTION:
   GAs are lousy at solving the travelling salesperson problem.  But here is an
example of how to do it anyway.  In this case we use a steady-state genetic
algorithm and give you the compile-time choice of partial match or edge 
recombination crossover.
   This one looks really good when you draw an entire population of individuals
and watch them all evolve in real time.  It becomes very obvious what the
genetic algorithm is doing and how well the speciation is working (or not).
   This implementation is by no means efficient, so please do not complain 
about all of the silly little inefficient aspects of the implementation.  But 
it does get the job done.
---------------------------------------------------------------------------- */
#include "ex26.hpp"

int main(int argc, char **argv)
{
  std::cout << "Example 26\n\n";
  std::cout << "This program tries to solve a travelling salesperson problem\n";
  std::cout << "using a steady-state GA and a list genome.\n\n";
  std::cout.flush();

  unsigned int seed = 0;
  std::string tspFile = TSP_FILE;
  for (int ii = 1; ii < argc; ii++)
  {
    if (strcmp(argv[ii], "seed") == 0 && ii + 1 < argc)
      seed = (unsigned int)atoi(argv[++ii]);
    else if ((strcmp(argv[ii], "file") == 0 || strcmp(argv[ii], "f") == 0) && ii + 1 < argc)
      tspFile = argv[++ii];
  }

  auto stats = example26(seed, tspFile);

  std::cout << "the ga generated the following path (score is "
            << stats.bestIndividual().score() << "):\n"
            << stats.bestIndividual() << "\n";
  std::cout << "\nthe statistics for the run are:\n" << stats << "\n";

  return 0;
}
