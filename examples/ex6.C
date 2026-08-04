#include "ex6.hpp"

#include <GASStateGA.h>
#include <GATreeGenome.hpp>
#include <iostream>

// Objective (moved from header)
float objectiveEx6(GAGenome &c)
{
	auto &genome = (GATreeGenome<int> &)c;
	return genome.size();
}

// Initializer (moved from header)
void TreeInitializer(GAGenome &c)
{
	auto &child = (GATreeGenome<int> &)c;

	// destroy any pre-existing tree
	child.root();
	child.destroy();

	// Create a new tree with depth of 'depth' and each eldest node containing
	// 'n' children (the other siblings have none).
	int depth = 2, n = 3, count = 0;
	child.insert(count++, GATreeBASE::ROOT);

	for (int i = 0; i < depth; i++)
	{
		child.eldest();
		child.insert(count++);
		for (int j = 0; j < n; j++)
			child.insert(count++, GATreeBASE::AFTER);
	}
}

// WriteNode and template specialization moved into this translation unit
void WriteNode(std::ostream &os, GANode<int> *n)
{
	if (!n)
		return;
	auto *node = (GANodeBASE *)n;

	os.width(10);
	os << ((GANode<int> *)node)->contents << " ";
	os.width(10);
	if (node->parent)
		os << ((GANode<int> *)node->parent)->contents << " ";
	else
		os << "." << " ";
	os.width(10);
	if (node->child)
		os << ((GANode<int> *)node->child)->contents << " ";
	else
		os << "." << " ";
	os.width(10);
	if (node->next)
		os << ((GANode<int> *)node->next)->contents << " ";
	else
		os << "." << " ";
	os.width(10);
	if (node->prev)
		os << ((GANode<int> *)node->prev)->contents << "\n";
	else
		os << ".\n";
	WriteNode(os, (GANode<int> *)node->child);

	for (GANodeBASE *tmp = node->next; tmp && tmp != node; tmp = tmp->next)
	{
		os.width(10);
		os << ((GANode<int> *)tmp)->contents << " ";
		os.width(10);
		if (tmp->parent)
			os << ((GANode<int> *)tmp->parent)->contents << " ";
		else
			os << "." << " ";
		os.width(10);
		if (tmp->child)
			os << ((GANode<int> *)tmp->child)->contents << " ";
		else
			os << "." << " ";
		os.width(10);
		if (tmp->next)
			os << ((GANode<int> *)tmp->next)->contents << " ";
		else
			os << "." << " ";
		os.width(10);
		if (tmp->prev)
			os << ((GANode<int> *)tmp->prev)->contents << "\n";
		else
			os << ".\n";
		WriteNode(os, (GANode<int> *)tmp->child);
	}
}

template <> int GATreeGenome<int>::write(std::ostream &os) const
{
	os << "      node     parent      child       next       prev\n";
	WriteNode(os, (GANode<int> *)rt);
	return os.fail() ? 1 : 0;
}

// ex6 implementation (moved from header)
GATreeGenome<int> ex6(GAParameterList params, unsigned int seed)
{
	// Now create the GA and run it.  We first create a chromsome with the
	// operators we want.  Once we have the genome set up, create the genetic
	// algorithm, set the parameters, and let it go.
	GATreeGenome<int> genome(objectiveEx6);
	genome.initializer(TreeInitializer);
	genome.mutator(GATreeGenome<int>::SwapSubtreeMutator);

	GASteadyStateGA ga(genome);
	ga.parameters(params);
	ga.evolve(seed);

	genome = ga.statistics().bestIndividual();
	std::cout << genome.size() << " nodes, " << genome.depth() << " levels deep." << std::endl;
	std::cout << "best of generation data are in '" << ga.scoreFilename() << "'\n";

	return genome;
}

// Small wrappers (kept as before)
float objective(GAGenome &c)
{
	return objectiveEx6(c);
}

GAStatistics example6(GAParameterList params, unsigned int seed)
{
	GATreeGenome<int> genome(objective);
	genome.initializer(TreeInitializer);
	genome.mutator(GATreeGenome<int>::SwapSubtreeMutator);

	GASteadyStateGA ga(genome);
	ga.parameters(params);
	ga.evolve(seed);

	return ga.statistics();
}
