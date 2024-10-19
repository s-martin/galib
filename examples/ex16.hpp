#pragma once

#include <ga.h>
#include <iostream>

float objective(GAGenome &c)
{
  auto &genome = (GATreeGenome<Point> &)c;
  return genome.size();
}

void TreeInitializer(GAGenome &c)
{
  auto &child = (GATreeGenome<Point> &)c;
  child.root();
  child.destroy();

  int depth = 2, n = 3, count = 0;
  child.insert(Point(count++, count++), GATreeBASE::ROOT);

  for (int i = 0; i < depth; i++)
  {
    child.eldest();
    child.insert(Point(count++, count++));
    for (int j = 0; j < n; j++)
      child.insert(Point(count++, count++), GATreeBASE::AFTER);
  }
}

GAStatistics example16(unsigned int seed, int argc, char **argv)
{
  GATreeGenome<Point> genome(objective);
  genome.initializer(TreeInitializer);
  genome.crossover(GATreeGenome<Point>::OnePointCrossover);

  genome.mutator(GATreeGenome<Point>::SwapSubtreeMutator);
  GAPopulation swappop(genome, 50);

  genome.mutator(GATreeGenome<Point>::DestructiveMutator);
  GAPopulation destpop(genome, 50);

  GASteadyStateGA ga(genome);
  ga.nGenerations(10);

  ga.population(swappop);
  ga.initialize(seed);
  while (!ga.done())
  {
    ga.step();
  }

  genome = ga.statistics().bestIndividual();

  ga.population(destpop);
  ga.initialize(seed);
  while (!ga.done())
  {
    ga.step();
  }

  genome = ga.statistics().bestIndividual();

  return ga.statistics();
}
