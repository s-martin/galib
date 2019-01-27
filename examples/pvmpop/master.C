/* ----------------------------------------------------------------------------
  master.C
  mbwall 5dec95
  Copyright (c) 1995-1996 Massachusetts Institute of Technology

 DESCRIPTION:
   Example program to illustrate use of GAlib with PVM.  This example uses a
master-slave configuration to parallelize the genetic algorithm.  In this case,
the master controls the evolution by specifying migrations between populations
on various processes.
---------------------------------------------------------------------------- */
#include <cstdio>
#include <iostream.h>
#include "PVMDemeGA.h"
#include "genome.h"

int
main(int argc, char** argv) {
  std::cout << "This program tries to fill a 1DBinaryStringGenome with\n";
  std::cout << "alternating 1s and 0s using a simple genetic algorithm.  It runs\n";
  std::cout << "in parallel using PVM and a population on each process.\n\n";
  std::cout.flush();

  GA1DBinaryStringGenome genome(GENOME_LENGTH);
  PVMDemeGA ga(genome);
  ga.parameters(argc, argv);
  ga.parameters("settings.txt");
  if(ga.spawn("slave") < 0) exit(1);

  std::cout << "initializing..." <<  std::endl;
  ga.initialize();
  std::cout << ga.statistics().bestIndividual() <<  std::endl;
  std::cout << "evolving..." <<  std::endl;
  while(!ga.done()){
    ga.step();
    std::cout << ga.statistics().bestIndividual() <<  std::endl;
  }
  ga.flushScores();

  std::cout << "\nThe GA found an individual with a score of ";
  std::cout << ga.statistics().bestIndividual().score() <<  std::endl;

  return 0;
}
