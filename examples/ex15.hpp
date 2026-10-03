#pragma once

#include <ga.h>
#include <cmath>
#include <iostream>

// For this objective function we try to match the values in the array of float
// that is passed to us as userData.  The closer the values in the genome are
// to the values in the sequence, the better the score.
float objectiveEx15(GAGenome &c)
{
  auto &genome = (GABin2DecGenome &)c;
  auto *sequence = (float *)c.userData();

  float value = genome.nPhenotypes();
  for (unsigned int i = 0; i < genome.nPhenotypes(); i++)
    value += 1.0 / (1.0 + fabs(genome.phenotype(i) - sequence[i]));
  return value;
}

GAStatistics example15(unsigned int seed, int argc, char **argv)
{
  (void)argc;
  (void)argv;

  // Generate a sequence of random numbers using the values in the min and max
  // arrays.
  const int n = 5;
  float min[n] = { 0, 0, 3, -5, 100 };
  float max[n] = { 1, 100, 3, -2, 100000 };
  float target[n];

  GARandomSeed(seed);
  for (int i = 0; i < n; i++)
    target[i] = GARandomFloat(min[i], max[i]);

  std::cout << "input sequence:\n";
  for (int i = 0; i < n; i++)
  {
    std::cout.width(10);
    std::cout << target[i] << " ";
  }
  std::cout << "\n\n";
  std::cout.flush();

  // Create the phenotype map and the genome that the GA will use.
  GABin2DecPhenotype map;
  for (int i = 0; i < n; i++)
    map.add(8, min[i], max[i]);
  GABin2DecGenome genome(map, objectiveEx15, (void *)target);

  // Use convergence as the stopping criterion rather than the number of
  // generations.
  GASimpleGA ga(genome);
  ga.populationSize(50);
  ga.nGenerations(500);
  ga.pMutation(0.01);
  ga.pCrossover(0.6);
  ga.scoreFilename("bog.dat");
  ga.scoreFrequency(10);
  ga.flushFrequency(50);
  ga.terminator(GAGeneticAlgorithm::TerminateUponConvergence);
  ga.pConvergence(0.99);
  ga.nConvergence(20);
  ga.evolve(seed);

  return ga.statistics();
}
