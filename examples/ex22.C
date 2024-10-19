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
#include <cstdio>
#include <cmath>
#include <ga.h>
#include <iostream>
#include <fstream>
#include "ex22.hpp"

#define OBJECTIVE Objective1
#define MIN_VALUE (-100)
#define MAX_VALUE 100

//#define OBJECTIVE Objective2
//#define MIN_VALUE -50
//#define MAX_VALUE 50

float Objective1(GAGenome&);
float Objective2(GAGenome&);
int   Mutator(GAGenome&, float);
void  Initializer(GAGenome&);
float Comparator(const GAGenome&, const GAGenome&);
int   Crossover(const GAGenome&, const GAGenome&, GAGenome*);

// Here we define our own genetic algorithm class.  This class is almost the
// same as the steady-state genetic algorithm, but we modify the step method
// (the one that does all the work) so that we do a slightly modified 
// replacement.  We're only going to do a two-parents-make-one-child mating,
// so we define our own crossover and use it rather than the standard one in
// GAlib.

using SingleChildCrossover = int (*)(const GAGenome &, const GAGenome &, GAGenome *);

class SharedOverlapGA : public GASteadyStateGA {
public:
  GADefineIdentity("SharedOverlapGA", 200);
  SharedOverlapGA(const GAGenome& g) : GASteadyStateGA(g) {}
  ~SharedOverlapGA() override = default;
  void step() override;
  SharedOverlapGA & operator++() { step(); return *this; }
  void crossover(SingleChildCrossover func) {crossFunction = func;}
protected:
  SingleChildCrossover crossFunction;
};

// This step method is similar to that of the regular steady-state genetic
// algorithm, but here we generate only one child in a crossover, and we
// do a slightly different type of replacement.  Here we generate the new
// individuals, insert them into the population, force a scaling to occur, 
// then remove the worst individuals.  This is all done based on the scaled
// (fitness) scores, not the raw (objective) scores.

void
SharedOverlapGA::step()
{ 
  int i;
  GAGenome *mom, *dad;

  for(i=0; i<tmpPop->size(); i++){	// takes care of odd population
    mom = &(pop->select());  
    dad = &(pop->select());
    stats.numsel += 2;		// keep track of number of selections
    if(GAFlipCoin(pCrossover()))
      stats.numcro += crossFunction(*mom, *dad, &tmpPop->individual(i));
    else if(GARandomBit()) 
      tmpPop->individual(i).copy(*mom);
    else
      tmpPop->individual(i).copy(*dad);
    stats.nummut += tmpPop->individual(i).mutate(pMutation());
  }

  for(i=0; i<tmpPop->size(); i++)
    pop->add(tmpPop->individual(i));

  pop->evaluate();		// get info about current pop for next time
  pop->scale();			// remind the population to do its scaling

  for(i=0; i<tmpPop->size(); i++)
    pop->destroy(GAPopulation::WORST, GAPopulation::SCALED);

  stats.update(*pop);		// update the statistics by one generation
}

// Here are two different objective functions.  Function 1 has multiple peaks
// with significant difference between peak heights - it is a modulated
// sinusoid.  Function 2 has less difference between peaks - it is an 
// approximation of a square plateau using a sum of sinusoids.
float
Objective1(GAGenome& g)
{
  auto& genome = (GA1DArrayGenome<float>&)g;
  float v = genome.gene(0);
  float y = 100.0 * exp(-fabs(v) / 50.0) * (1.0 - cos(v * M_PI * 2.0 / 25.0));
  if(v < MIN_VALUE || v > MAX_VALUE) y = 0;
  if(y < 0) y = 0;
  return y+0.00001;
}

float
Objective2(GAGenome& g)
{
  auto& genome = (GA1DArrayGenome<float>&)g;
  float v = genome.gene(0) / 100.0;
  float y = 0.5 + 0.6 * sin(M_PI*v) + 0.2 * sin(3*M_PI*v) + 0.1 * sin(5*M_PI*v)
    + 0.02 * sin(7*M_PI*v) + 0.01 * sin(7*M_PI*v);
  if(v < -0.23 || v > 1.23) y = 0;
  if(y < 0) y = 0;
  return y+0.00001;
}

void
Initializer(GAGenome& g)
{
  auto& genome = (GA1DArrayGenome<float>&)g;
  genome.gene(0, GARandomFloat(-100.0, 100.0));
}

int
Mutator(GAGenome& g, float pmut)
{
  auto& genome = (GA1DArrayGenome<float>&)g;
  int nmut = 0;
  if(GAFlipCoin(pmut)){
    genome.gene(0, genome.gene(0) + 
		GARandomFloat() * (GARandomFloat() - GARandomFloat()));
    nmut = 1;
  }
  return nmut;
}

int
Crossover(const GAGenome& g1, const GAGenome& g2, GAGenome* c1)
{
  auto& mom = (GA1DArrayGenome<float>&)g1;
  auto& dad = (GA1DArrayGenome<float>&)g2;
  auto& child = (GA1DArrayGenome<float>&)*c1;

  float distance = 0.0, midpoint = 0.0;

  midpoint = (mom.gene(0) + dad.gene(0)) / 2;
  distance = fabs(mom.gene(0) - dad.gene(0));
  child.gene(0, midpoint + distance * (GARandomFloat() - GARandomFloat()));

  return 1;
}

// You can change the factor to control how tightly the distance function 
// considers the spacing of two genomes.  Higher numbers will give you a 
// tighter clustering at function peaks.

#define FACTOR 800

float
Comparator(const GAGenome& g1, const GAGenome& g2) 
{
  auto& a = (GA1DArrayGenome<float>&)g1;
  auto& b = (GA1DArrayGenome<float>&)g2;

  float val= exp( - (a.gene(0)-b.gene(0)) * (a.gene(0)-b.gene(0)) / FACTOR);
  if(1-val < 0 || 1-val > 1)  std::cerr << "val: " << val << "\n";
  return 1-val;
}
