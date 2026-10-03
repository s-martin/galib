/* ----------------------------------------------------------------------------
  ex12.C
  mbwall 13apr95
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example shows how to use an order-based string genome.
---------------------------------------------------------------------------- */
#include "ex12.hpp"

int main(int argc, char *argv[])
{
  std::cout << "Example 12\n\n";
  std::cout << "This program illustrates the use of order-based strings.  The\n";
  std::cout << "string in this problem contains 26 letters, a to z.  It tries\n";
  std::cout << "to put them in alphabetic order.\n\n";
  std::cout.flush();

  unsigned int seed = 0;
  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii++],"seed") == 0) {
      seed = (unsigned int)atoi(argv[ii]);
    }
  }

  auto stats = example12(seed, argc, argv);

  auto genome = static_cast<const GAStringGenome &>(stats.bestIndividual());
  std::cout << "the ga generated the following string (objective score is ";
  std::cout << genome.score() << "):\n" << genome << "\n";
  std::cout << genome.className() << "\n";

  return 0;
}
