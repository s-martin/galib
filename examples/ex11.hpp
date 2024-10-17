#pragma once

#include <ga.h>
#include <iostream>

float objective(GAGenome &c)
{
  auto &genome = (GAListGenome<int> &)c;

  float score = (*genome.head() == genome.size() - 1); // move to head of list
  for (int i = genome.size() - 2; i > 0; i--)
    score += (*genome.next() == i); // cycle through list and look at nodes
  return score;
}

void ListInitializer(GAGenome &c)
{
  auto &child = (GAListGenome<int> &)c;
  while (child.head())
    child.destroy(); // destroy any pre-existing list

  int n = 25;
  child.insert(0, GAListBASE::HEAD); // the head node contains a '0'
  for (int i = 1; i < n; i++)
    child.insert(i); // each subsequent node contains a number
  for (int j = 0; j < n; j++)
    child.swap(GARandomInt(0, n - 1), GARandomInt(0, n - 1));
}

GAStatistics example11(unsigned int seed, bool useStatic)
{
  std::cout << "Example 11\n\n";
  std::cout << "This program illustrates the use of order-based lists.  The\n";
  std::cout << "list in this problem contains 25 numbers, 0 to 24.  It tries\n";
  std::cout << "to put them in descending order from 24 to 0.\n\n";
  std::cout.flush();

  if (useStatic)
  {
    GARandomSeed(seed);
  }

  GAParameterList params;
  GASteadyStateGA::registerDefaultParameters(params);
  params.set(gaNpopulationSize, 30); // population size
  params.set(gaNpCrossover, 0.6); // probability of crossover
  params.set(gaNpMutation, 0.01); // probability of mutation
  params.set(gaNnGenerations, 1000); // number of generations
  params.set(gaNpReplacement, 0.5); // how much of pop to replace each gen
  params.set(gaNscoreFrequency, 10); // how often to record scores
  params.set(gaNnReplacement, 4); // how much of pop to replace each gen
  params.set(gaNflushFrequency, 10); // how often to dump scores to file
  params.set(gaNscoreFilename, "bog.dat");

  GAListGenome<int> genome(objective);
  genome.initializer(ListInitializer);
  genome.mutator(GAListGenome<int>::SwapMutator);

  GASteadyStateGA ga(genome);
  ga.crossover(GAListGenome<int>::PartialMatchCrossover);
  ga.parameters(params);
  ga.evolve();

  genome = ga.statistics().bestIndividual();
  std::cout << "the ga generated the following list (objective score is ";
  std::cout << genome.score() << "):\n"
            << genome << "\n";
  std::cout << "best of generation data are in '" << ga.scoreFilename() << "'\n";
  std::cout << ga.parameters() << "\n";

  return ga.statistics();
}

template <>
int GAListGenome<int>::write(std::ostream &os) const
{
  int *cur, *head;
  GAListIter<int> tmpiter(*this);
  if ((head = tmpiter.head()) != nullptr)
    os << *head << " ";
  for (cur = tmpiter.next(); cur && cur != head; cur = tmpiter.next())
    os << *cur << " ";
  return os.fail() ? 1 : 0;
}
