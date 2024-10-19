/* ----------------------------------------------------------------------------
  ex23.C
  mbwall 5jan96
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example shows how to use max/min feature of GAlib to maximize or 
minimize your objective functions.
---------------------------------------------------------------------------- */
#include <cstdio>
#include <cmath>
#include <ga.h>
#include <iostream>
#include <fstream>
#include "ex23.hpp"

#define INSTANTIATE_REAL_GENOME
#include <GARealGenome.h>

#define MIN_VALUE (-2)
#define MAX_VALUE 2
#define INC       0.005

float Objective(GAGenome &);
float Comparator(const GAGenome&, const GAGenome&);

int main(int argc, char** argv) {
  example23(argc, argv);
  return 0;
}

// This objective function returns the sin of the value in the genome.
float Objective(GAGenome& g){
  auto& genome = (GARealGenome &)g;
  return 1 + sin(genome.gene(0)*2*M_PI);
}

// The comparator returns a number in the interval [0,1] where 0 means that
// the two genomes are identical (zero diversity) and 1 means they are 
// completely different (maximum diversity).
float Comparator(const GAGenome& g1, const GAGenome& g2) {
  auto& a = (GARealGenome &)g1;
  auto& b = (GARealGenome &)g2;
  return exp( -(a.gene(0) - b.gene(0)) * (a.gene(0) - b.gene(0)) / 1000.0);
}
