/* ----------------------------------------------------------------------------
  ex20.C
  mbwall 5sep95
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example runs the royal road problem.  See the comments near the 
objective functions for details about the function itself.
   Some of this was copied (at least partially) from the galopps genetic
algorithm library and from the pga package.  I used a bunch of globals in this
example - not good programming style, but it gets the job done.
---------------------------------------------------------------------------- */
#include "ex20.hpp"

int main(int argc, char *argv[])
{
  unsigned int seed = 0;
  for (int ii = 1; ii < argc; ii++)
  {
    if (strcmp(argv[ii++], "seed") == 0)
      seed = (unsigned int)atoi(argv[ii]);
  }

  example20(seed, argc, argv);

  return 0;
}
