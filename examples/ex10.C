#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ga.h>
#include "ex10.hpp"

#include <iostream>
#include <fstream>

#define USE_RAW_SINE

#define NBITS     8

#ifdef USE_RAW_SINE
#define FUNCTION  Function1
#define MIN_VALUE 0
#define MAX_VALUE 5
#else
#define FUNCTION  Function2
#define MIN_VALUE -100
#define MAX_VALUE 100
#endif

float Function1(float);
float Function2(float);
float Objective(GAGenome &);
float BitDistance(const GAGenome & a, const GAGenome & b);
float PhenotypeDistance(const GAGenome & a, const GAGenome & b);

int main(int argc, char **argv)
{
    example10(0, argc, argv);
    return 0;
}

// You can choose between one of two sinusoidal functions.  The first one has
// peaks of equal amplitude.  The second is modulated.
float
Objective(GAGenome & c){
  auto & genome = (GABin2DecGenome &)c;
  return FUNCTION (genome.phenotype(0));
}

float
Function1(float v) {
  return 1 + sin(v*2*M_PI);
}

float
Function2(float v) {
  float y;
  y = 100.0 * exp(-fabs(v) / 50.0) * (1.0 - cos(v * M_PI * 2.0 / 25.0));
  if(v < -100 || v > 100) y = 0;
  if(y < 0) y = 0;
  return y+0.00001;
}

// Here are a couple of possible distance functions for this problem.  One of
// them uses the genes to determine the same-ness, the other uses the
// phenotypes to determine same-ness.  If the genomes are the same, then
// we return a 0.  If they are completely different then we return a 1.
//   In either case, you should be sure that the distance function will return
// values only between 0 and 1 inclusive.  If your function returns values
// outside these limits, the GA will produce bogus results and it WILL NOT warn
// you that your distance function is brain-dead!

// This distance function uses the genes to determine same-ness.  All we do 
// is check to see if the bit strings are identical.

float
BitDistance(const GAGenome & c1, const GAGenome & c2){
  auto & a = (GABin2DecGenome &)c1;
  auto & b = (GABin2DecGenome &)c2;

  float x=0;
  for(int i=a.length()-1; i>=0; i--)
    x += (a[i] != b[i] ? 1 : 0);

  return x/a.length();
}

// This distance function looks at the phenotypes rather than the genes of the
// genome.  This distance function will try to drive them to extremes.

float
PhenotypeDistance(const GAGenome & c1, const GAGenome & c2){
  auto & a = (GABin2DecGenome &)c1;
  auto & b = (GABin2DecGenome &)c2;

  return fabs(a.phenotype(0) - b.phenotype(0)) / (MAX_VALUE-MIN_VALUE);
}
