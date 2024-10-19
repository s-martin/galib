/* ----------------------------------------------------------------------------
  ex16.C
  mbwall 5may95
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Illustration of how to use a non-trivial object in the nodes of a tree
genome.  This example uses points in the nodes.
---------------------------------------------------------------------------- */
#include <cstdio>
#include <cstdlib>
#include <ga.h>
#include "ex16.hpp"

#include <iostream>

int main(int argc, char **argv)
{
  std::cout << "Example 16\n\n";
  std::cout << "This example uses a SteadyState GA and Tree<int> genome.  It\n";
  std::cout << "tries to maximize the size of the tree genomes that it\n";
  std::cout << "contains.  The genomes contain points in its nodes.  Two\n";
  std::cout << "different runs are made:  first with the swap subtree mutator,\n";
  std::cout << "second with the destructive mutator.\n\n";
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

  GATreeGenome<Point> genome(objective);
  genome.initializer(TreeInitializer);
  genome.crossover(GATreeGenome<Point>::OnePointCrossover);

  genome.mutator(GATreeGenome<Point>::SwapSubtreeMutator);
  GAPopulation swappop(genome, 50);

  genome.mutator(GATreeGenome<Point>::DestructiveMutator);
  GAPopulation destpop(genome, 50);

  GASteadyStateGA ga(genome);
  ga.nGenerations(10);

  // first do evolution with subtree swap mutator.

  ga.population(swappop);

  std::cout << "initializing...";
  ga.initialize(seed);
  std::cout << "evolving for " << ga.nGenerations() << " generations...";
  while (!ga.done())
  {
    ga.step();
    std::cout << ".";
    std::cout.flush();
  }
  std::cout << "\n";

  genome = ga.statistics().bestIndividual();
  std::cout << "the ga generated a tree with " << genome.size();
  std::cout << " nodes, " << genome.depth() << " levels deep.\n";

  // now do evolution with destructive swap mutator

  ga.population(destpop);

  std::cout << "\ninitializing...";
  ga.initialize();
  std::cout << "evolving for " << ga.nGenerations() << " generations...";
  while (!ga.done())
  {
    ga.step();
    std::cout << ".";
    std::cout.flush();
  }
  std::cout << "\n";

  genome = ga.statistics().bestIndividual();
  std::cout << "the ga generated a tree with " << genome.size();
  std::cout << " nodes, " << genome.depth() << " levels deep.\n";

  return 0;
}
