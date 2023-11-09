#pragma once

#include <GASStateGA.h>
#include <GAListGenome.hpp>

#include <iostream>


/* ----------------------------------------------------------------------------
Objective function
  There is no limit to the size of a list (only the memory you have on your
computer).  This objective function tries to build a list that contains the
number 101 in all of its nodes.  If we don't put some control on this objective
then the list will grow without bound.  So we dampen it a bit with a penalty
for large size.  However, this will make the score go negative, so we must use
a scaling object that allows negative objective scores.
  We could get lists with no contents, so we have to check for that.  Just make
sure that head has contents.  If it does, then the rest of the genome will.
  When we access the node contents we have to dereference the member functions.
For example, we do *chrom.head() not chrom.head().  This is because the member
functions return a pointer to the node's contents, not the actual contents.
---------------------------------------------------------------------------- */
float objectiveEx8(GAGenome &c)
{
	auto &genome = (GAListGenome<int> &)c;
	int count = 0;
	if (!genome.head())
		return 0;
	count = (*genome.head() == 101) ? 1 : 0; // move to head of the list
	for (int i = 1; i < genome.size(); i++)
		count +=
			(*genome.next() == 101) ? 1 : 0; // check each element of the list
	return 5 * count - genome.size();
}

/* ----------------------------------------------------------------------------
Here is the initializer for our genomes.  It builds a list of n items of type
int.  Notice that we first destroy any list that is already in the genome
before we do our initialization.  This is so that the genomes can be re-used.
When you re-run a GA, it does not destroy the individuals in the population -
it reuses them.  Thus, the initializer must make sure that the genome is
cleaned up before it tries to initialize it.
---------------------------------------------------------------------------- */
void ListInitializer(GAGenome &c)
{
	auto &child = (GAListGenome<int> &)c;

	while (child.head())
		child.destroy(); // destroy any pre-existing list

	int n = 75;
	child.insert(100, GAListBASE::HEAD);
	for (int i = 0; i < n; i++)
		child.insert(i + 100);
	for (int j = 0; j < n; j++)
		child.swap(GARandomInt(0, n - 1), GARandomInt(0, n - 1));
}

//   Here we specialize the write method for the List class.  This lets us see
// exactly what we want (the default write method dumps out pointers to the
// data rather than the data contents).
//   This routine prints out the contents of each element of the list,
// separated by a space.  It does not put a newline at the end of the list.
//   Notice that you can specialize ANY function of a template class, but
// some compilers are more finicky about how you do it than others.  For the
// metrowerks compiler this specialization must come before the forced
// instantiation.
// TODO fix double instantiation
//template <> int GAListGenome<int>::write(std::ostream &os) const
//{
//	int *cur, *head;
//	GAListIter<int> tmpiter(*this);
//	if ((head = tmpiter.head()) != nullptr)
//		os << *head << " ";
//	for (cur = tmpiter.next(); cur && cur != head; cur = tmpiter.next())
//		os << *cur << " ";
//	return os.fail() ? 1 : 0;
//}

GAListGenome<int> ex8()
{
	// Create the initial genome for the genetic algorithm to use.  Set the
	// initializer and mutation before we make the genetic algorithm.
	GAListGenome<int> genome(objectiveEx8);
	genome.initializer(ListInitializer);
	//  genome.mutator(GAListGenome<int>::SwapMutator);
	genome.mutator(GAListGenome<int>::DestructiveMutator);

	// Now that we have a genome, we use it to create our GA.  After creating
	// the GA we set the parameters and tell the GA to use sigma truncation
	// scaling rather than the default (linear scaling).  Set the crossover to
	// single point crossover.  The genetic algorithm handles crossover since
	// genomes don't know about other genomes.  We could set the crossover on
	// the genome if we wanted - either way will work.
	GASteadyStateGA ga(genome);
	GASigmaTruncationScaling scale;
	ga.scaling(scale);
	ga.crossover(GAListGenome<int>::OnePointCrossover);

	// Set the default parameters we want to use, then check the command line
	// for other arguments that might modify these.
	ga.set(gaNpopulationSize, 40); // population size
	ga.set(gaNpCrossover, 0.6); // probability of crossover
	ga.set(gaNpMutation, 0.05); // probability of mutation
	ga.set(gaNnGenerations, 50); // number of generations
	ga.set(gaNscoreFrequency, 1); // how often to record scores
	ga.set(gaNflushFrequency, 10); // how often to dump scores to file
	ga.set(gaNselectScores, // which scores should we track?
		GAStatistics::Maximum | GAStatistics::Minimum | GAStatistics::Mean);
	ga.set(gaNscoreFilename, "bog.dat");
    // TODO	ga.parameters(argc, argv);

	// Evolve the genetic algorithm then dump out the results of the run.
	ga.evolve();

	genome = ga.statistics().bestIndividual();
	//  std::cout << "the ga generated the list:\n" << genome << "\n";
	std::cout << "the list contains " << genome.size() << " nodes\n";
	std::cout << "the ga used the parameters:\n" << ga.parameters() << "\n";

	return genome;
}