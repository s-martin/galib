/* ----------------------------------------------------------------------------
  ex25.C
  mbwall jan96
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example shows how to do multiple populations on a single CPU.
---------------------------------------------------------------------------- */
#include <cstdio>
#include <GA1DBinStrGenome.h>
#include <GADemeGA.h>
#include <iostream>
#include "ex25.hpp"

float Objective(GAGenome &);

int main(int argc, char** argv) {
  std::cout << "Example 25\n\n";
  std::cout << "This example uses a genetic algorithm with multiple populations.\n";
  std::cout <<  std::endl;

  unsigned int seed = 0;
  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii++],"seed") == 0) {
      seed = atoi(argv[ii]);
    }
  }

  GAStatistics stats = example25(seed, argc, argv);
  std::cout << "best individual is: " << stats.bestIndividual() << "\n";
  std::cout << "\n" << stats << "\n";  

  return 0;
}

// This is the 1-max objective function - try to maximize the number of 1s in 
// a bit string of arbitrary length.
float Objective(GAGenome& g) {
  auto & genome = (GA1DBinaryStringGenome &)g;
  float score=0.0;
  for(int i=0; i<genome.length(); i++)
    score += genome.gene(i);
  score /= genome.length();
  return score;
}
