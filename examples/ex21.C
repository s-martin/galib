/* ----------------------------------------------------------------------------
  ex21.C
  mbwall 1jan96
  Copyright (c) 1995-1996 Massachusetts Institute of Technology

 DESCRIPTION:
   This example illustrates various uses of the AlleleSet in concert with the
ArrayAlleleGenome.  In particular, we use the RealGenome to show how you can
use the enumerated and bounded types of allele sets.
   You can define one allele set for an entire array, or you can define one
allele set for each element in the array.  The constructor that you use to 
create the array determines which behaviour you'll get.
---------------------------------------------------------------------------- */
#include <cstdio>
#include <ga.h>
#include "ex21.hpp"

#include <iostream>

#define INSTANTIATE_REAL_GENOME
#include <GARealGenome.h>

int main(int argc, char** argv)
{
  example21(0, argc, argv);
  return 0;
}
