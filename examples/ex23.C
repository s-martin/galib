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
 
 
#define INSTANTIATE_REAL_GENOME
#include <GARealGenome.h>

#define MIN_VALUE (-2)
#define MAX_VALUE 2
#define INC       0.005

float Objective(GAGenome &);
float Comparator(const GAGenome&, const GAGenome&);

int
main(int argc, char** argv)
{
  std::cout << "Example 23\n\n";
  std::cout << "This program tries to maximize or minimize, depending on the\n";
  std::cout << "command line argument that you give it.  Use the command-line\n";
  std::cout << "argument 'mm -1' to minimize (the default for this example), or\n";
  std::cout << "'mm 1' to maximize.  The objective function is a simple \n";
  std::cout << "sinusoidal.\n\n"; std::cout.flush();

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  for(int jj=1; jj<argc; jj++) {
    if(strcmp(argv[jj++],"seed") == 0) {
      GARandomSeed((unsigned int)atoi(argv[jj]));
    }
  }

  std::ofstream outfile;

  GARealAlleleSet alleles(MIN_VALUE, MAX_VALUE);
  GARealGenome genome(1, alleles, Objective);
  GASharing scale(Comparator);

  auto params = std::make_shared<GAParameterList>();
  params->set(gaNpopulationSize, 50);	// how many individuals in the population
  params->set(gaNnGenerations, 25);		// number of generations to evolve
  params->set(gaNpMutation, 0.001);		// likelihood of mutating new offspring
  params->set(gaNpCrossover, 0.9);		// likelihood of crossing over parents
  params->set(gaNscoreFilename, "bog.dat");	// name of file for scores
  params->set(gaNscoreFrequency, 1);		// keep the scores of every generation
  params->set(gaNflushFrequency, 10);	// specify how often to write the score to disk
  params->parse(argc, argv);

  GASimpleGA ga(genome, params);
  ga.minimize();		// by default we want to minimize the objective
  ga.scaling(scale);		// set the scaling method to our sharing
  ga.selectScores(GAStatistics::AllScores);
  ga.initialize();

// dump the initial population to file

  std::cout << "printing initial population to file..." <<  std::endl;
  outfile.open("popi.dat", (std::ios::out | std::ios::trunc));
  for(int ii=0; ii<ga.population().size(); ii++){
    genome = ga.population().individual(ii);
    outfile << genome.gene(0) << "\t" << genome.score() << "\n";
  }
  outfile.close();

  while(!ga.done()) ga.step();

// dump the final population to file

  std::cout << "printing final population to file..." <<  std::endl;
  outfile.open("popf.dat", (std::ios::out | std::ios::trunc));
  for(int i=0; i<ga.population().size(); i++){
    genome = ga.population().individual(i);
    outfile << genome.gene(0) << "\t" << genome.score() << "\n";
  }
  outfile.close();

// dump the function to file so you can plot the population on it

  std::cout << "printing function to file..." <<  std::endl;
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
  return exp( -(a.gene(0) - b.gene(0)) * (a.gene(0) - b.gene(0)) / 1000.0);
}
