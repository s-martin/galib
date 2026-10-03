#pragma once

#include <ga.h>
#include <iostream>

#define INSTANTIATE_STRING_GENOME
#include <GAStringGenome.h>

float objectiveEx12(GAGenome &c)
{
  auto &genome = (GAStringGenome &)c;

  float score = 0;
  for (int i = 0; i < genome.length(); i++)
    score += (genome.gene(i) == 'a' + i);
  return score;
}

GAStatistics example12(unsigned int seed, int argc, char **argv)
{
  std::cout << "Example 12\n\n";
  std::cout << "This program illustrates the use of order-based strings.  The\n";
  std::cout << "string in this problem contains 26 letters, a to z.  It tries\n";
  std::cout << "to put them in alphabetic order.\n\n";
  std::cout.flush();

  GAParameterList params;
  GASimpleGA::registerDefaultParameters(params);
  params.set(gaNpopulationSize, 30); // population size
  params.set(gaNpCrossover, 0.6); // probability of crossover
  params.set(gaNpMutation, 0.01); // probability of mutation
  params.set(gaNnGenerations, 1000); // number of generations
  params.set(gaNscoreFrequency, 10); // how often to record scores
  params.set(gaNflushFrequency, 50); // how often to dump scores to file
  params.set(gaNscoreFilename, "bog.dat");

  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii++],"seed") == 0) {
      seed = (unsigned int)atoi(argv[ii]);
    }
  }

  // The allele set contains the letters a to z.  The ordered initializer puts
  // each of them in the string exactly once.
  GAAlleleSet<char> letters;
  for (int i = 0; i < 26; i++)
    letters.add('a' + i);

  GAStringGenome genome(26, letters, objectiveEx12);
  genome.initializer(GAStringGenome::OrderedInitializer);
  genome.crossover(GAStringGenome::PartialMatchCrossover);
  genome.mutator(GAStringGenome::SwapMutator);

  GASimpleGA ga(genome);
  ga.parameters(params);
  ga.evolve(seed);

  return ga.statistics();
}
