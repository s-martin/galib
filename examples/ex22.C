/* ----------------------------------------------------------------------------
  ex22.C
  mbwall 5jan96
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example shows how to derive your own genetic algorithm class.  This one
does a modified form of speciation that is useful for fitness-scaled speciation
with overlapping populations (Goldberg's speciation is designed for use with 
non-overlapping populations.  
   The steady-state genetic algorithm built-in to GAlib is actually capable of
doing this already, but this example illustrates how you can modify a genetic
algorithm to do your own thing.  For example, instead of using the "single
child crossover" you could use your own crossover algorithm instead.
---------------------------------------------------------------------------- */
#include "ex22.hpp"

int main(int argc, char **argv)
{
  std::cout << "Example 22\n\n";
  std::cout << "This program illustrates the use of a derived genetic algorithm\n";
  std::cout << "class: a steady-state GA with a shared (speciated) replacement.\n\n";
  std::cout.flush();

  unsigned int seed = 0;
  for (int ii = 1; ii < argc; ii++)
  {
    if (strcmp(argv[ii++], "seed") == 0)
      seed = (unsigned int)atoi(argv[ii]);
  }

  example22(seed, argc, argv);

  return 0;
}
