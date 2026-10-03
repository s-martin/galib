/* ----------------------------------------------------------------------------
  ex23.C
  mbwall 5jan96
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example shows how to use max/min feature of GAlib to maximize or 
minimize your objective functions.
---------------------------------------------------------------------------- */
#include "ex23.hpp"

int main(int argc, char **argv)
{
  unsigned int seed = 0;
  for (int ii = 1; ii < argc; ii++)
  {
    if (strcmp(argv[ii++], "seed") == 0)
      seed = (unsigned int)atoi(argv[ii]);
  }

  example23(seed, argc, argv);
  return 0;
}
