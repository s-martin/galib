/* ----------------------------------------------------------------------------
  ex11.C
  mbwall 13apr95
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example shows how to use an order-based list genome.
---------------------------------------------------------------------------- */
#include "ex11.hpp"

int main(int argc, char **argv)
{
  // See if we've been given a seed to use (for testing purposes).  When you
  // specify a random seed, the evolution will be exactly the same each time
  // you use that seed number.
  unsigned int seed = 0;
  for (int ii = 1; ii < argc; ii++)
  {
    if (strcmp(argv[ii++], "seed") == 0)
      seed = (unsigned int)atoi(argv[ii]);
  }

  example11(seed, seed != 0);
  return 0;
}
