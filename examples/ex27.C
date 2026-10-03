/* ----------------------------------------------------------------------------
  ex27.C
  mbwall 24mar96
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

  The code for this example is adapted from an original implementation 
  by Thomas Grueninger (from Uni Stuttgart, visiting scholar at MIT)

 DESCRIPTION:
   This example shows how to use the Deterministic Crowding genetic algorithm.
You can specify one of 4 different functions (they are described in more detail
later in this file).
   (This code was originally used with a 3D OpenGL-based program to illustrate
the differences in convergence between various speciation methods.  Believe me,
it looks much better running in real-time in 3D, but, alas, there is not yet
any standard 3D cross-platform API, so you get this instead.)
---------------------------------------------------------------------------- */
#include "ex27.hpp"

int main(int argc, char **argv)
{
  unsigned int seed = 0;
  for (int ii = 1; ii < argc; ii++)
  {
    if (strcmp(argv[ii++], "seed") == 0)
      seed = (unsigned int)atoi(argv[ii]);
  }

  example27(seed, argc, argv);

  return 0;
}
