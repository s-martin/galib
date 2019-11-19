/* ----------------------------------------------------------------------------
  ex24.C
  mbwall 5jan96
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example shows how to restricted mating using a custom genetic algorithm
and custom selection scheme.  The restricted mating in the genetic algorithm
tries to pick individuals that are similar (based upon their comparator).  The
selector chooses only the upper half of the population (so it is very selective
and tends to drive the convergence faster than roulette wheel selection, for
example).
---------------------------------------------------------------------------- */
#include <cstdio>
#include <cmath>
#include <ga.h>
 

#include <iostream>
#include <fstream>
 

#define INSTANTIATE_REAL_GENOME
#include <GARealGenome.h>

#define MIN_VALUE (-2)
#define MAX_VALUE 2
#define INC       0.005

#define THRESHOLD 0.5


float Objective(GAGenome &);
float Comparator(const GAGenome&, const GAGenome&);





// Use any ID above 200 for the object's ID number.  The selection scheme
// takes care of keeping the population up-to-date - we only have to worry
// about the selection method itself.  This selector picks randomly, but only
// from the upper half of the population (based on the scaled fitness scores).
class FinickySelector : public GASelectionScheme {
public:
  GADefineIdentity("FinickySelector", 273);
  FinickySelector(int w=GASelectionScheme::SCALED) : GASelectionScheme(w) { }
  FinickySelector(const FinickySelector& orig)  : GASelectionScheme(orig) { copy(orig); }
  FinickySelector& operator=(const FinickySelector& orig) 
    { if(&orig != this) copy(orig); return *this; }
  ~FinickySelector() override = default;
  GASelectionScheme* clone() const override { return new FinickySelector; }
  GAGenome& select() const override;
};

GAGenome&
FinickySelector::select() const {
  return pop->best(GARandomInt(0, pop->size()/2), GAPopulation::SCALED); 
}




// This is the genetic algorithm that does the restricted mating.  It is 
// similar to the steady state genetic algorithm, but we modify the selection
// part of the step method to do the restricted mating.

class RestrictedMatingGA : public GASteadyStateGA {
public:
  GADefineIdentity("RestrictedMatingGA", 288);
  RestrictedMatingGA(const GAGenome& g) : GASteadyStateGA(g) {}
  ~RestrictedMatingGA() override = default;
  void step() override;
  RestrictedMatingGA & operator++() { step(); return *this; }
};

// This step method is similar to that of the regular steady-state genetic
// algorithm, but here we select only individuals that are similar to each
// other (based on what their comparators tell us).

void
RestrictedMatingGA::step()
{ 
  int i;
  GAGenome *mom, *dad;

// Generate the individuals in the temporary population from individuals in 
// the main population.  We do a restrictive selection in which we only let
// individuals that are dissimilar mate.  We do a little count just to make
// sure that we don't loop forever (just accept whatever we selected if we
// can't find what we'd like).

  for(i=0; i<tmpPop->size()-1; i+=2){	// takes care of odd population
    mom = &(pop->select()); 
    int k=0;
    do { k++; dad = &(pop->select()); }
    while(mom->compare(*dad) < THRESHOLD && k<pop->size());
    stats.numsel += 2;		// keep track of number of selections
    if(GAFlipCoin(pCrossover())){
      stats.numcro += (*scross)(*mom, *dad, &tmpPop->individual(i), 
				&tmpPop->individual(i+1));
    }
    else{
      tmpPop->individual( i ).copy(*mom);
      tmpPop->individual(i+1).copy(*dad);
    }
    stats.nummut += tmpPop->individual( i ).mutate(pMutation());
    stats.nummut += tmpPop->individual(i+1).mutate(pMutation());
  }
  if(tmpPop->size() % 2 != 0){	// do the remaining population member
    mom = &(pop->select());  
    dad = &(pop->select());
    int k=0;
    do { k++; dad = &(pop->select()); }
    while(mom->compare(*dad) < THRESHOLD && k<pop->size());
    stats.numsel += 2;		// keep track of number of selections
    if(GAFlipCoin(pCrossover())){
      stats.numcro += (*scross)(*mom, *dad,
				&tmpPop->individual(i), nullptr);
    }
    else{
      if(GARandomBit()) tmpPop->individual( i ).copy(*mom);
      else tmpPop->individual( i ).copy(*dad);
    }
    stats.nummut += tmpPop->individual( i ).mutate(pMutation());
  }

// Now stick the new individuals into the population, force an evaluation,
// force a scaling, then remove the worst individuals so that we keep a 
// constant population size.

  for(i=0; i<tmpPop->size(); i++)
    pop->add(tmpPop->individual(i));

  pop->evaluate();		// get info about current pop for next time
  pop->scale();			// remind the population to do its scaling

  for(i=0; i<tmpPop->size(); i++)
    pop->destroy(GAPopulation::WORST, GAPopulation::SCALED);

  stats.update(*pop);		// update the statistics by one generation
}








int
main(int argc, char** argv)
{
  std::cout << "Example 24\n\n";
  std::cout << "This example illustrates how to derive your own genetic\n";
  std::cout << "algorithm.  This genetic algorithm does restricted mating and\n";
  std::cout << "uses a selector slightly more finicky than a uniform random\n";
  std::cout << "selector.  The objective function is a simple sinusoidal.\n\n";
  std::cout.flush();

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  unsigned int seed = 0;
  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii++],"seed") == 0) {
      seed = atoi(argv[ii]);
    }
  }

  GARealAlleleSet alleles(MIN_VALUE, MAX_VALUE);
  GARealGenome genome(1, alleles, Objective);
  GASharing scale(Comparator);
  FinickySelector select;

  RestrictedMatingGA ga(genome);
  ga.selector(select);		// use our selector instead of default
  ga.scaling(scale);		// set the scaling method to our sharing
  ga.populationSize(50);	// how many individuals in the population
  ga.nGenerations(200);		// number of generations to evolve
  ga.pMutation(0.001);		// likelihood of mutating new offspring
  ga.pCrossover(0.9);		// likelihood of crossing over parents
  ga.scoreFilename("bog.dat");	// name of file for scores
  ga.scoreFrequency(1);		// keep the scores of every generation
  ga.flushFrequency(50);	// specify how often to write the score to disk
  ga.selectScores(GAStatistics::AllScores);
  ga.parameters("settings.txt"); // read parameters from settings file
  ga.parameters(argc, argv, true); // parse commands, complain if bogus args
  ga.evolve(seed);

  std::ofstream outfile;

// dump the final population to file

  std::cout << "printing population to file 'population.dat'..." <<  std::endl;
  outfile.open("population.dat", (std::ios::out | std::ios::trunc));
  for(int i=0; i<ga.population().size(); i++){
    genome = ga.population().individual(i);
    outfile << genome.gene(0) << "\t" << genome.score() << "\n";
  }
  outfile.close();

// dump the function to file so you can plot the population on it

  std::cout << "printing function to file 'sinusoid.dat'..." <<  std::endl;
  outfile.open("sinusoid.dat", (std::ios::out | std::ios::trunc));
  for(float x=MIN_VALUE; x<=MAX_VALUE; x+=INC){
    outfile << genome.gene(0,x) << "\t" << genome.score() << "\n";
  }
  outfile.close();

  return 0;
}
 



// This objective function returns the sin of the value in the genome.
float
Objective(GAGenome& g){
  auto& genome = (GARealGenome &)g;
  return 1 + sin(genome.gene(0)*2*M_PI);
}

// The comparator returns a number in the interval [0,1] where 0 means that
// the two genomes are identical (zero diversity) and 1 means they are 
// completely different (maximum diversity).
float
Comparator(const GAGenome& g1, const GAGenome& g2) {
  auto& a = (GARealGenome &)g1;
  auto& b = (GARealGenome &)g2;
  return exp( -(a.gene(0) - b.gene(0)) * (a.gene(0) - b.gene(0)) / 100.0);
}
