#pragma once

#include <ga.h>
#include <iostream>

// A simple non-trivial object to store in the nodes of the tree.
class Point
{
public:
  Point(int xx = 0, int yy = 0) : x(xx), y(yy) {}
  int x, y;
};

inline std::ostream &operator<<(std::ostream &os, const Point &p)
{
  return os << "(" << p.x << ", " << p.y << ")";
}

float objectiveEx16(GAGenome &c)
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
  GATreeGenome<Point> genome(objectiveEx16);
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
