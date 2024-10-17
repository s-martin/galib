#ifndef EX26_HPP
#define EX26_HPP

#include <GASStateGA.h>
#include <GAListGenome.hpp>
#include <garandom.h>

#define MAX_TOWNS 50
#define TSP_FILE "tsp_rect_20.txt"

extern float DISTANCE[MAX_TOWNS][MAX_TOWNS];
extern double x[MAX_TOWNS], y[MAX_TOWNS];
extern int ntowns;

float objective(GAGenome& g) {
  auto & genome = (GAListGenome<int> &)g;
  float dist = 0;
  if(genome.head()) {
    for(int i=0; i<ntowns; i++) {
      int xx = *genome.current();
      int yy = *genome.next();
      dist += DISTANCE[xx][yy];
    }
  }
  return dist;
}

GASimpleGA::Statistics example26(unsigned int seed, const std::vector<std::string>& args) {
  GARandomSeed(seed);

  GAListGenome<int> genome(objective);
  genome.initializer(Initializer);
  genome.mutator(Mutator);
  genome.crossover(XOVER);

  GASteadyStateGA ga(genome);
  ga.populationSize(30);
  ga.nGenerations(100);
  ga.pMutation(0.01);
  ga.pCrossover(0.9);
  ga.scoreFilename("bog.dat");
  ga.scoreFrequency(10);
  ga.flushFrequency(50);
  ga.selectScores(GAStatistics::AllScores);
  ga.evolve();

  return ga.statistics();
}

#endif
