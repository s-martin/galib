/* ----------------------------------------------------------------------------
  ex16.C
  mbwall 5may95
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Illustration of how to use a non-trivial object in the nodes of a tree
genome.  This example uses points in the nodes.
---------------------------------------------------------------------------- */
#include "ex16.hpp"

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

  example16(seed, argc, argv);

  return 0;
}
